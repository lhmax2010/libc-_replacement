#!/bin/bash

# Disable pathname expansion by '*' or '?'
set -f

IFS=";"

# Special keyword for unspecified attribute
UNSPECIFIED="Unspecified"

# Message formatting
LINE_MAX_LENGTH=90

# Deprecated
ROOTSTRAP_BASE_DIR="/etc/app-rootstrap"
# Rootstrap-checker rule file path
ROOTSTRAP_DEFAULT_RULES_DIR="/etc/app-rootstrap/default.rules.d"
ROOTSTRAP_RULES_DIR="/etc/app-rootstrap/rules.d"

# Deprecated: Base dir only reads this specific yaml file
ROOTSTRAP_BASE_DIR_YAML="app-rootstrap-checker.yaml"

# Current Package Info (Global)
CURRENT_GROUP_NAME=""
CURRENT_PACKAGE_NAME=""
CURRENT_PACKAGE_SPECFILE=""
SOURCEDIR=""
RPM_BUILD_ROOT=""
BUILD_ARCH=""
BUILD_BIT=""

print_border() {
	local placeholder=$(printf "%*s" "$LINE_MAX_LENGTH" " ")
	echo ${placeholder// /\#}
}

print_message() {
	local message_max_length=$(($LINE_MAX_LENGTH - 3))
	printf "# %-*s#\n" "$message_max_length" "$1"
}

print_not_allowed_files_log() {
	local target_dir=$1
	local not_allowed_files=$2
	local allowed_files=$3
	local action=$4

	if [[ "$action" == "ignore" ]]; then
		return
	fi

	local prefix="WARNING"
	if [[ "$action" == "error" ]]; then
		prefix="ERROR"
	fi

	print_border
	print_border
	print_message "$prefix: Target path: $target_dir"
	print_message "$prefix: There are not allowed files installed."
	local not_allowed_file=""
	for not_allowed_file in $not_allowed_files; do
		print_message "$prefix:  - $not_allowed_file"
	done
	print_message "$prefix:"
	print_message "$prefix: Only allowed files below can be installed:"
	local allowed_file=""
	for allowed_file in $allowed_files; do
		print_message "$prefix:  - $allowed_file"
	done
	print_border
	print_border
}

print_not_allowed_log() {
	local warning_type=$1
	local not_allowed_list=$2
	local allowed_package_name_list=$3
	local allowed_group_name_list=$4
	local action=$5

	if [[ "$action" == "ignore" ]]; then
		return
	fi

	local prefix="WARNING"
	if [[ "$action" == "error" ]]; then
		prefix="ERROR"
	fi

	local not_allowed=""
	local allowed=""

	print_border
	print_border

	print_message "$prefix: There are not allowed $warning_type. They might cause ABI break."
	for not_allowed in ${not_allowed_list}; do
		local not_allowed_package_name=""
		local not_allowed_group_name=""
		if [[ $not_allowed =~ (.*)@(.*) ]]; then
			not_allowed_package_name=${BASH_REMATCH[1]}
			not_allowed_group_name=${BASH_REMATCH[2]}
		fi
		print_message "$prefix:  - $not_allowed_package_name, Group: $not_allowed_group_name"
	done

	print_message "$prefix:"
	if [[ ! -z $allowed_package_name_list ]]; then
		print_message "$prefix: Use only allowed packages below for $warning_type"
		for allowed in ${allowed_package_name_list}; do
			print_message "$prefix:  - $allowed"
		done
	else
		print_message "$prefix: It is not allowed to specify $warning_type"
	fi

	if [[ ! -z $allowed_group_name_list ]]; then
		print_message "$prefix: Use only allowed package groups below for $warning_type"
		for allowed in ${allowed_group_name_list}; do
			print_message "$prefix:  - $allowed"
		done
	fi

	print_border
	print_border
}

ACTION_ON_VIOLATION=""
TARGET_PACKAGE_NAME=""
TARGET_GROUP_NAME=""
TARGET_INSTALLED_FILE=""
TARGET_BUILDREQUIRES=""
EXCLUDED_TARGET_PACKAGE_NAME=""
ALLOWED_BUILDREQUIRES_PACKAGE_NAME=""
ALLOWED_BUILDREQUIRES_GROUP_NAME=""
ALLOWED_BUILDCONFLICTS_PACKAGE_NAME=""
ALLOWED_REQUIRES=""
DENIED_LDD_LIST=""
DIR_KEYS=()
DIR_VALUES=()

reset_config_variables() {
	ACTION_ON_VIOLATION="warning"
	TARGET_PACKAGE_NAME=""
	TARGET_GROUP_NAME=""
	TARGET_INSTALLED_FILE=""
	TARGET_BUILDREQUIRES=""
	EXCLUDED_TARGET_PACKAGE_NAME=""
	ALLOWED_BUILDREQUIRES_PACKAGE_NAME=""
	ALLOWED_BUILDREQUIRES_GROUP_NAME=""
	ALLOWED_BUILDCONFLICTS_PACKAGE_NAME=""
	ALLOWED_REQUIRES=""
	DENIED_LDD_LIST=""
	DIR_KEYS=()
	DIR_VALUES=()
}

get_all_config_files() {
	local final_file_list=""
	local OLD_IFS="$IFS"
	IFS=$'\n'

	# Check if there are any files in ROOTSTRAP_BASE_DIR
	local base_dir_files
	base_dir_files=$(find "$ROOTSTRAP_BASE_DIR" -maxdepth 1 -type f -name "*.yaml" 2>/dev/null)

	if [[ -n "$base_dir_files" ]]; then
		# Case 1: ROOTSTRAP_BASE_DIR has at least one file
		# Ignore both rules.d and default.rules.d
		for filepath in $base_dir_files; do
			final_file_list+="$filepath$OLD_IFS"
		done
		IFS="$OLD_IFS"
		echo "$final_file_list"
		return
	fi

	# Check if there are any files in ROOTSTRAP_RULES_DIR
	local rules_dir_files
	rules_dir_files=$(find "$ROOTSTRAP_RULES_DIR" -maxdepth 1 -type f -name "*.yaml" 2>/dev/null)

	if [[ -n "$rules_dir_files" ]]; then
		# Case 2: ROOTSTRAP_BASE_DIR is empty and ROOTSTRAP_RULES_DIR has at least one file
		# Ignore default.rules.d
		for filepath in $rules_dir_files; do
			final_file_list+="$filepath$OLD_IFS"
		done
		IFS="$OLD_IFS"
		echo "$final_file_list"
		return
	fi

	# Case 3: Both ROOTSTRAP_BASE_DIR and ROOTSTRAP_RULES_DIR are empty
	# Use files from ROOTSTRAP_DEFAULT_RULES_DIR
	local default_rules_files
	default_rules_files=$(find "$ROOTSTRAP_DEFAULT_RULES_DIR" -maxdepth 1 -type f -name "*.yaml" 2>/dev/null)

	for filepath in $default_rules_files; do
		final_file_list+="$filepath$OLD_IFS"
	done

	IFS="$OLD_IFS"
	echo "$final_file_list"
}

# Parse critical configuration fields for early skip condition check.
# This function parses action_on_violation to determine
# whether to skip parsing the remaining configuration fields.
# Returns 0 if the key should continue first-pass parsing, 1 to stop first-pass parsing.
parse_config_action_on_violation_property() {
	case $KEY in
		action_on_violation)
			ACTION_ON_VIOLATION="$VALUE"
			return 0
			;;
		*)
			# We have already parsed enough information to decide
			# whether to stop or not.
			return 1
			;;
	esac
}

# Parse critical configuration fields for early skip condition check.
# This function parse target_package_name, and target_group_name
# whether to skip parsing the remaining configuration fields.
# Returns 0 if the key should continue first-pass parsing, 1 to stop first-pass parsing.
parse_config_target_package_name_and_group_name_property() {
	case $KEY in
		target_package_name)
			TARGET_PACKAGE_NAME+="$VALUE$IFS"
			return 0
			;;
		target_group_name)
			TARGET_GROUP_NAME+="$VALUE$IFS"
			return 0
			;;
		*)
			# We have already parsed enough information to decide
			# whether to stop or not.
			return 1
			;;
	esac
}

# Parse critical configuration fields for early skip condition check.
# This function parse target_installed_file
# whether to skip parsing the remaining configuration fields.
# Returns 0 if the key should continue first-pass parsing, 1 to stop first-pass parsing.
parse_config_target_installed_file_property() {
	case $KEY in
		target_installed_file)
			TARGET_INSTALLED_FILE+="$VALUE$IFS"
			return 0
			;;
		*)
			# We have already parsed enough information to decide
			# whether to stop or not.
			return 1
			;;
	esac
}

# Parse critical configuration fields for early skip condition check.
# This function parses target_buildrequires
# whether to skip parsing the remaining configuration fields.
# Returns 0 if the key should continue first-pass parsing, 1 to stop first-pass parsing.
parse_config_target_buildrequires_property() {
	case $KEY in
		target_buildrequires)
			TARGET_BUILDREQUIRES+="$VALUE$IFS"
			return 0
			;;
		*)
			# We have already parsed enough information to decide
			# whether to stop or not.
			return 1
			;;
	esac
}

# Parse configuration properties for allowed and excluded packages/dependencies.
# Sets corresponding global variables (EXCLUDED_TARGET_PACKAGE_NAME, ALLOWED_BUILDREQUIRES_PACKAGE_NAME, etc.)
parse_allowed_and_excluded_property() {
	local line=$1

	case $KEY in
		excluded_target_package_name)
			EXCLUDED_TARGET_PACKAGE_NAME+="$VALUE$IFS"
			;;
		allowed_buildrequires_package_name)
			ALLOWED_BUILDREQUIRES_PACKAGE_NAME+="$VALUE$IFS"
			;;
		allowed_buildrequires_group_name)
			ALLOWED_BUILDREQUIRES_GROUP_NAME+="$VALUE$IFS"
			;;
		allowed_buildconflicts_package_name)
			ALLOWED_BUILDCONFLICTS_PACKAGE_NAME+="$VALUE$IFS"
			;;
		allowed_requires)
			ALLOWED_REQUIRES+="$VALUE$IFS"
			;;
		allowed_installed_file)
			parse_allowed_file "$VALUE"
			;;
		denied_ldd)
			DENIED_LDD_LIST+="$VALUE$IFS"
			;;
		*)
			echo "Undefined key($KEY), skip the line: $line"
			;;
	esac
}

get_key_and_value_from_config() {
	local line="$1"

	VALUE=""
	if [[ "$line" =~ ^(.*):\ (.*)$ ]]; then # 'key: value'
		KEY="${BASH_REMATCH[1]}"
		VALUE="${BASH_REMATCH[2]}"
	elif [[ "$line" =~ ^(.*):$ ]]; then # 'key:'
		KEY="${BASH_REMATCH[1]}"
		VALUE=""
	elif [[ "$line" =~ ^-\ (.*)$ ]]; then # '- value'
		VALUE="${BASH_REMATCH[1]}"
	fi
}

# Returns 0 if we can skip checker, 1 to keep parsing the configuration.
is_required_to_skip_checker() {
	if [[ "$ACTION_ON_VIOLATION" == "ignore" ]]; then
		return 0
	fi

	# Instead of repeatedly checking the check_target_criteria,
	# it always returns 1 if there is any match in check_target_criteria.
	if [[ "$MATCHED_TARGET_CRITERIA" == 1 ]]; then
		return 1
	fi

	# Do not skip the configuration parsing.
	# target_package_name, target_group_name, target_installed_file, and target_buildrequires
	# might still be unparsed.
	if [[ -z "$TARGET_PACKAGE_NAME" && -z "$TARGET_GROUP_NAME" && -z "$TARGET_INSTALLED_FILE" && -z "$TARGET_BUILDREQUIRES" ]]; then
		return 1
	fi

	# Check if current package matches target criteria
	if ! check_target_criteria; then
		return 0
	fi

	# This indicates that the current package matches the target criteria.
	MATCHED_TARGET_CRITERIA=1

	return 1
}

# Add a file mapping entry in the allowed files mapping table.
# Maintains a mapping of target directories to allowed filenames.
# If the target directory already exists in the mapping, appends the new filename.
# If the target directory is new, creates a new entry.
add_allowed_files_mapping() {
	local target_dir=$1
	local filename=$2
	local i

	for ((i=0; i<${#DIR_KEYS[@]}; i++)); do
		if [ "${DIR_KEYS[$i]}" = "$target_dir" ]; then
			DIR_VALUES[$i]="${DIR_VALUES[$i]};$filename"
			return
		fi
	done

	DIR_KEYS+=("$target_dir")
	DIR_VALUES+=("$filename")
}

# Parse allowed file specification from configuration.
# Expected format: "<bit>bit, <full_path>"
# Extracts architecture bit (32/64) and full path, then validates
# if the bit matches the current BUILD_BIT.
# If matched, extracts target directory and filename, and adds to mapping.
parse_allowed_file() {
	local line=$1

	if [[ $line =~ ^([0-9]+)bit,\ (.+)$ ]]; then
		local bit="${BASH_REMATCH[1]}"
		local full_path="${BASH_REMATCH[2]}"

		if [[ "$bit" == "$BUILD_BIT" ]]; then
			local target_dir=${full_path%/*}
			local filename=${full_path##*/}

			add_allowed_files_mapping "$target_dir" "$filename"
		fi
	fi
}

parse_config_and_check_skip_condition() {
	local config_file=$1

	MATCHED_TARGET_CRITERIA=0
	# Naive yaml parser for configuration file
	KEY=""
	while read LINE; do
		# Trim comment(#) and whitespace
		LINE="${LINE%%#*}"
		LINE="${LINE#"${LINE%%[![:space:]]*}"}"
		LINE="${LINE%"${LINE##*[![:space:]]}"}"

		if [[ -z "$LINE" ]]; then
			continue
		fi

		get_key_and_value_from_config "$LINE"

		if [[ -z "$VALUE" ]]; then
			continue
		fi

		if parse_config_action_on_violation_property; then
			continue
		elif parse_config_target_package_name_and_group_name_property; then
			continue
		elif parse_config_target_installed_file_property; then
			continue
		elif parse_config_target_buildrequires_property; then
			continue
		else
			if is_required_to_skip_checker; then
				return 0
			fi
		fi

		parse_allowed_and_excluded_property "$LINE"
	done < $config_file

	return 1
}

check_matching() {
	local input_string=$1
	local match_string_list=$2
	local is_matched=0
	local string=""

	for string in $match_string_list; do
		if [[ "$input_string" == $string ]]; then
			is_matched=1
			break
		fi
	done

	echo $is_matched
}

# Check matching of installed_files and target_file.
# Returns 0 if it is not matched, 1 if it is matched.
check_matching_of_installed_files() {
	local target_file=$1
	local installed_files=$2
	local installed_file=""
	local is_matched=0

	for installed_file in $installed_files; do
		if [[ $(check_matching "$installed_file" "$target_file") == 1 ]]; then
			is_matched=1
			break
		fi
	done

	echo $is_matched
}

# Check whether files exists or not in the target directory
# Returns 0 if it exists, 1 if it does not exist.
check_installed_file_existence_of_target() {
	local target_installed_files=$1
	local target_installed_file

	for target_installed_file in $target_installed_files; do
		local target_dir=${target_installed_file%/*}
		local target_file=${target_installed_file##*/}
		local installed_files=$(get_installed_files_for_pattern "$target_dir")

		if [[ $(check_matching_of_installed_files "$target_file" "$installed_files") == 1 ]]; then
			return 0
		fi
	done

	return 1
}

check_target_criteria() {
	# Rule inspection criteria keywords list
	local criteria_keys="target_package_name;target_group_name;target_installed_file;target_buildrequires"
	local key

	for key in $criteria_keys; do
		case $key in
			"target_package_name")
				if [[ -n "$TARGET_PACKAGE_NAME" ]]; then
					if [[ $(check_matching "$CURRENT_PACKAGE_NAME" "$TARGET_PACKAGE_NAME") == 1 ]]; then
						return 0 # Matched
					fi
				fi
				;;
			"target_group_name")
				if [[ -n "$TARGET_GROUP_NAME" ]]; then
					if [[ $(check_matching "$CURRENT_GROUP_NAME" "$TARGET_GROUP_NAME") == 1 ]]; then
						return 0 # Matched
					fi
				fi
				;;
			"target_installed_file")
				if check_installed_file_existence_of_target "$TARGET_INSTALLED_FILE"; then
					return 0 # Matched
				fi
				;;
			"target_buildrequires")
				if [[ -n "$TARGET_BUILDREQUIRES" ]]; then
					if check_buildrequires_matching_target; then
						return 0 # Matched
					fi
				fi
				;;
		esac
	done

	return 1 # Not matched
}

# Check if current package's BuildRequires matches any pattern in TARGET_BUILDREQUIRES.
# Parses the spec file for BuildRequires entries and compares each
# against the target_buildrequires patterns using bash pattern matching.
# Returns 0 if matched, 1 if not matched.
check_buildrequires_matching_target() {
	local parsed_buildrequires_list
	parsed_buildrequires_list=$(get_spec_buildrequires "$CURRENT_PACKAGE_SPECFILE")
	local br_entry=""

	for br_entry in $parsed_buildrequires_list; do
		local br_package_name=""
		if [[ $br_entry =~ (.*)@(.*) ]]; then
			br_package_name="${BASH_REMATCH[1]}"
		fi

		if [[ $(check_matching "$br_package_name" "$TARGET_BUILDREQUIRES") == 1 ]]; then
			return 0 # Matched
		fi
	done

	return 1 # Not matched
}

collect_not_allowed() {
	local dependency_list=$1
	local allowed_package_name_list=$2
	local allowed_group_name_list=$3
	local not_allowed_dependency_list=""
	local dependency=""

	if [[ -z "$allowed_package_name_list" ]]; then
		echo "$not_allowed_dependency_list"
		return
	fi

	for dependency in $dependency_list; do
		local dependency_package_name=""
		local dependency_group_name=""
		if [[ $dependency =~ (.*)@(.*) ]]; then
			local dependency_package_name=${BASH_REMATCH[1]}
			local dependency_group_name=${BASH_REMATCH[2]}
		fi

		local package_name_pattern_matched=$(check_matching "$dependency_package_name" "$allowed_package_name_list")
		if [[ $package_name_pattern_matched == 1 ]]; then
			continue
		fi

		if [[ $dependency_group_name != $UNSPECIFIED ]]; then
			local group_name_pattern_matched=$(check_matching "$dependency_group_name" "$allowed_group_name_list")
			if [[ $group_name_pattern_matched == 1 ]]; then
				continue
			fi
		fi

		not_allowed_dependency_list+="$dependency$IFS"
	done

	echo "$not_allowed_dependency_list"
}

check_dependencies_against_config() {
	local parsed_buildrequires_list=$(get_spec_buildrequires "$CURRENT_PACKAGE_SPECFILE")
	local parsed_buildconficts_list=$(get_spec_buildconflicts "$CURRENT_PACKAGE_SPECFILE")
	local parsed_requires_list=$(get_spec_requires "$CURRENT_PACKAGE_SPECFILE")

	local violate_buildrequires=$(collect_not_allowed "$parsed_buildrequires_list" "$ALLOWED_BUILDREQUIRES_PACKAGE_NAME" "$ALLOWED_BUILDREQUIRES_GROUP_NAME")
	local violate_buildconflicts=$(collect_not_allowed "$parsed_buildconficts_list" "$ALLOWED_BUILDCONFLICTS_PACKAGE_NAME" "")

	local has_violation=0

	if [[ -n "$violate_buildrequires" ]]; then
		print_not_allowed_log "BuildRequires" "$violate_buildrequires" "$ALLOWED_BUILDREQUIRES_PACKAGE_NAME" "$ALLOWED_BUILDREQUIRES_GROUP_NAME" "$ACTION_ON_VIOLATION"
		has_violation=1
	fi
	if [[ -n "$violate_buildconflicts" ]]; then
		print_not_allowed_log "BuildConflicts" "$violate_buildconflicts" "$ALLOWED_BUILDCONFLICTS_PACKAGE_NAME" "" "$ACTION_ON_VIOLATION"
		has_violation=1
	fi

	if [[ $has_violation == 1 && "$ACTION_ON_VIOLATION" == "error" ]]; then
		return 1
	fi
	return 0
}

get_installed_files_for_pattern() {
	local dir_pattern="$1"
	local installed_files=""
	local full_pattern="$RPM_BUILD_ROOT$dir_pattern/*"

	while IFS= read -r -d '' path; do
		local file_name="${path##*/}"
		installed_files+="$file_name$IFS"
	done < <(
		find "$RPM_BUILD_ROOT" -type f -path "$full_pattern" -print0 2>/dev/null
	)

	echo "$installed_files"
}

get_library_from_json() {
	local json_file=$1
	local lib_filenames=""

	local lib_values=$(grep -s -o '"lib"[[:space:]]*:[[:space:]]*"[^"]*"' "$json_file" | sed 's/.*"lib"[[:space:]]*:[[:space:]]*"\([^"]*\)".*/\1/')
	local lib_value=""
	while IFS= read -r lib_value; do
		if [[ -n "$lib_value" ]]; then
			local lib_filename=${lib_value##*/}
			lib_filenames+="$lib_filename$IFS"
		fi
	done <<< "$lib_values"

	echo "$lib_filenames"
}

get_build_arch_type() {
	case "$BUILD_ARCH" in
		aarch64|riscv64|x86_64)
			BUILD_BIT="64"
			;;
		*)
			BUILD_BIT="32"
			;;
	esac
}

check_allowed_file() {
	local file_name=$1
	local allowed_files=$2
	if [[ $(check_matching "$file_name" "$allowed_files") == 1 ]]; then
		return 0
	fi

	return 1
}

# Check if library files referenced in JSON files are allowed.
# Searches for JSON files in the target directory and extracts
# library filenames from "lib" fields.
# Compares extracted library filenames against the allowed files list.
# Returns a semicolon-separated list of not allowed library files.
check_allowed_files_in_json() {
	local dir_pattern="$1"
	local allowed_files=$2
	local not_allowed_libs=""
	local full_path_pattern="$RPM_BUILD_ROOT$dir_pattern/*.json"
	local json_files=$(find "$RPM_BUILD_ROOT" -type f -path "$full_path_pattern" 2>/dev/null)
	local json_file=""
	while IFS= read -r json_file; do
		local lib_filenames=$(get_library_from_json "$json_file")
		local lib_filename=""

		if [[ -z "$lib_filenames" ]]; then
			continue
		fi

		for lib_filename in $lib_filenames; do
			if ! check_allowed_file "$lib_filename" "$allowed_files"; then
				local json_filename=${json_file##*/}
				not_allowed_libs+="$lib_filename (from $json_filename)$IFS"
			fi
		done
	done <<< "$json_files"

	echo "$not_allowed_libs"
}

# Check if installed files in the target directory are allowed.
# Scans the target directory for all installed files and compares
# each filename against the allowed files list.
# Returns a semicolon-separated list of not allowed file names.
check_allowed_files_in_installed_files() {
	local target_dir=$1
	local allowed_files=$2
	local not_allowed_files=""

	local installed_files=$(get_installed_files_for_pattern "$target_dir")
	local installed_file=""

	for installed_file in $installed_files; do
        	if ! check_allowed_file "$installed_file" "$allowed_files"; then
			not_allowed_files+="$installed_file$IFS"
		fi
	done

	echo "$not_allowed_files"
}

check_allowed_files_against_config() {
	local has_violation=0

	for ((i=0; i<${#DIR_KEYS[@]}; i++)); do
		local target_dir="${DIR_KEYS[$i]}"
		local allowed_files="${DIR_VALUES[$i]}"

		local not_allowed_installed_files=$(check_allowed_files_in_installed_files "$target_dir" "$allowed_files")
		local not_allowed_libs=$(check_allowed_files_in_json "$target_dir" "$allowed_files")

		if [[ -n "$not_allowed_installed_files" || -n "$not_allowed_libs" ]]; then
			local not_allowed_files="$not_allowed_installed_files$not_allowed_libs"
			print_not_allowed_files_log "$target_dir" "$not_allowed_files" "$allowed_files" "$ACTION_ON_VIOLATION"
			has_violation=1
		fi
	done

	if [[ $has_violation == 1 && "$ACTION_ON_VIOLATION" == "error" ]]; then
		return 1
	fi
	return 0
}

print_denied_ldd_log() {
	local denied_ldd_violations=$1
	local action=$2

	if [[ "$action" == "ignore" ]]; then
		return
	fi

	local prefix="WARNING"
	if [[ "$action" == "error" ]]; then
		prefix="ERROR"
	fi

	print_border
	print_border
	print_message "$prefix: Denied LDD usage detected!"
	print_message "$prefix: The following binaries link to denied libraries."
	print_message "$prefix: This may indicate usage of non-standard API access methods."

	local violation=""
	for violation in $denied_ldd_violations; do
		print_message "$prefix:  - $violation"
	done

	print_message "$prefix:"
	print_message "$prefix: Denied LDD list:"
	local ldd=""
	for ldd in $DENIED_LDD_LIST; do
		print_message "$prefix:  - $ldd"
	done

	print_border
	print_border
}

is_elf_file() {
	local file_path=$1
	local file_type

	file_type=$(file -b "$file_path" 2>/dev/null)
	if [[ "$file_type" =~ ELF ]]; then
		return 0
	fi
	return 1
}

get_elf_needed_libraries() {
	local elf_file=$1
	local needed_libs=""

	# Use readelf to extract dynamic section and grep for NEEDED entries
	local needed_entries
	needed_entries=$(readelf -d "$elf_file" 2>/dev/null | grep "NEEDED" | sed 's/.*\[\(.*\)\]/\1/')

	local lib
	while IFS= read -r lib; do
		if [[ -n "$lib" ]]; then
			needed_libs+="$lib$IFS"
		fi
	done <<< "$needed_entries"

	echo "$needed_libs"
}

# Check if a pattern is a path pattern (starts with /)
is_path_pattern() {
	local pattern=$1
	if [[ "$pattern" == /* ]]; then
		return 0
	fi
	return 1
}

# Get all library files matching a path pattern.
# Searches in both RPM_BUILD_ROOT and system paths.
# Returns a semicolon-separated list of library filenames.
get_libraries_in_path_pattern() {
	local path_pattern=$1
	local lib_names=""

	# Extract directory path and filename pattern
	# e.g., /usr/lib64/hal/libhal-api-common.so* -> dir=/usr/lib64/hal, pattern=libhal-api-common.so*
	local dir_path="${path_pattern%/*}"
	local file_pattern="${path_pattern##*/}"

	# First, search in RPM_BUILD_ROOT (files installed by current package)
	if [[ -n "$RPM_BUILD_ROOT" ]]; then
		local full_dir="$RPM_BUILD_ROOT$dir_path"
		if [[ -d "$full_dir" ]]; then
			while IFS= read -r -d '' file_path; do
				local lib_name="${file_path##*/}"
				if [[ -n "$lib_name" ]]; then
					lib_names+="$lib_name$IFS"
				fi
			done < <(
				find "$full_dir" -type f -name "$file_pattern" -print0 2>/dev/null
			)
		fi
	fi

	# Also search in system path (for libraries from other packages)
	if [[ -d "$dir_path" ]]; then
		# Use -type f -o -type l to include both regular files and symlinks
		while IFS= read -r -d '' file_path; do
			local lib_name="${file_path##*/}"
			if [[ -n "$lib_name" ]]; then
				lib_names+="$lib_name$IFS"
			fi
		done < <(
			find "$dir_path" \( -type f -o -type l \) -name "$file_pattern" -print0 2>/dev/null
		)
	fi

	echo "$lib_names"
}

# Check if a library name matches any pattern in the denied list.
# Supports both library name patterns (e.g., "libsecret.so*") and
# path patterns (e.g., "/hal/*").
check_ldd_against_denied_list() {
	local lib_name=$1
	local denied_list=$2
	local is_denied=0
	local pattern=""

	for pattern in $denied_list; do
		# Check if pattern is a path pattern
		if is_path_pattern "$pattern"; then
			# Get all libraries in the path pattern
			local libs_in_path=$(get_libraries_in_path_pattern "$pattern")
			# Check if lib_name matches any library in the path
			if [[ $(check_matching "$lib_name" "$libs_in_path") == 1 ]]; then
				is_denied=1
				break
			fi
		else
			# Use bash pattern matching for library name
			if [[ "$lib_name" == $pattern ]]; then
				is_denied=1
				break
			fi
		fi
	done

	echo $is_denied
}

check_denied_ldd_usage() {
	local violations=""
	local elf_file=""

	# Find all ELF files in RPM_BUILD_ROOT
	while IFS= read -r -d '' elf_file; do
		# Skip if not an ELF file
		if ! is_elf_file "$elf_file"; then
			continue
		fi

		# Get the relative path from RPM_BUILD_ROOT
		local rel_path="${elf_file#$RPM_BUILD_ROOT}"

		# Get NEEDED libraries
		local needed_libs=$(get_elf_needed_libraries "$elf_file")
		local lib=""

		for lib in $needed_libs; do
			if [[ $(check_ldd_against_denied_list "$lib" "$DENIED_LDD_LIST") == 1 ]]; then
				violations+="$rel_path links to $lib$IFS"
			fi
		done
	done < <(
		find "$RPM_BUILD_ROOT" -type f -print0 2>/dev/null
	)

	echo "$violations"
}

# Build a list of library names to search for in strings.
# For path patterns, gets actual library names from the path.
# For name patterns, uses the pattern directly.
get_ldd_search_patterns() {
	local search_patterns=""
	local pattern=""

	for pattern in $DENIED_LDD_LIST; do
		if is_path_pattern "$pattern"; then
			# Get actual library names from the path
			local libs_in_path=$(get_libraries_in_path_pattern "$pattern")
			search_patterns+="$libs_in_path"
		else
			# Use the pattern directly
			search_patterns+="$pattern$IFS"
		fi
	done

	echo "$search_patterns"
}

check_dlopen_denied_ldd_usage() {
	local violations=""
	local elf_file=""

	# Get expanded list of library names to search for
	local search_patterns=$(get_ldd_search_patterns)

	# Find all ELF files in RPM_BUILD_ROOT
	while IFS= read -r -d '' elf_file; do
		# Skip if not an ELF file
		if ! is_elf_file "$elf_file"; then
			continue
		fi

		# Get the relative path from RPM_BUILD_ROOT
		local rel_path="${elf_file#$RPM_BUILD_ROOT}"

		# Extract strings from the binary
		local binary_strings
		binary_strings=$(strings "$elf_file" 2>/dev/null)

		# Check for dlopen patterns with denied LDD names
		local ldd_name=""
		for ldd_name in $search_patterns; do
			# Check if the LDD name appears in strings (potential dlopen usage)
			if echo "$binary_strings" | grep -qF "$ldd_name"; then
				# Also check if dlopen symbol is referenced
				if echo "$binary_strings" | grep -q "dlopen"; then
					violations+="$rel_path may use dlopen for $ldd_name$IFS"
				fi
			fi
		done
	done < <(
		find "$RPM_BUILD_ROOT" -type f -print0 2>/dev/null
	)

	echo "$violations"
}

check_strings_denied_ldd_usage() {
	local violations=""
	local elf_file=""

	# Get expanded list of library names to search for
	local search_patterns=$(get_ldd_search_patterns)

	# Find all ELF files in RPM_BUILD_ROOT
	while IFS= read -r -d '' elf_file; do
		# Skip if not an ELF file
		if ! is_elf_file "$elf_file"; then
			continue
		fi

		# Get the relative path from RPM_BUILD_ROOT
		local rel_path="${elf_file#$RPM_BUILD_ROOT}"

		# Extract strings from the binary
		local binary_strings
		binary_strings=$(strings "$elf_file" 2>/dev/null)

		# Check for denied LDD names in strings
		local ldd_name=""
		for ldd_name in $search_patterns; do
			# Search for the LDD name in strings
			if echo "$binary_strings" | grep -qF "$ldd_name"; then
				violations+="$rel_path contains string '$ldd_name'$IFS"
			fi
		done
	done < <(
		find "$RPM_BUILD_ROOT" -type f -print0 2>/dev/null
	)

	echo "$violations"
}

check_all_denied_ldd_usage() {
	local all_violations=""

	# Check direct linking
	local link_violations=$(check_denied_ldd_usage)
	if [[ -n "$link_violations" ]]; then
		all_violations+="$link_violations"
	fi

	# Check dlopen usage
	local dlopen_violations=$(check_dlopen_denied_ldd_usage)
	if [[ -n "$dlopen_violations" ]]; then
		all_violations+="$dlopen_violations"
	fi

	# Check string literals
	local string_violations=$(check_strings_denied_ldd_usage)
	if [[ -n "$string_violations" ]]; then
		all_violations+="$string_violations"
	fi

	echo "$all_violations"
}

check_denied_ldd_against_config() {
	# Skip if no denied_ldd is defined
	if [[ -z "$DENIED_LDD_LIST" ]]; then
		return 0
	fi

	local violations=$(check_all_denied_ldd_usage)

	if [[ -n "$violations" ]]; then
		print_denied_ldd_log "$violations" "$ACTION_ON_VIOLATION"
		if [[ "$ACTION_ON_VIOLATION" == "error" ]]; then
			return 1
		fi
	fi

	return 0
}

get_current_package_specfile() {
	local sourcedir=$1
	local result

	# Screen out packages that are not subject to the rootstrap-checker
	result=$(find "$sourcedir" -name *.spec)
	if [[ -z "$result" ]]; then
		return 1
	fi

	echo "$result"
}

get_current_package_metadata() {
	local specfile=$1
	CURRENT_PACKAGE_NAME=$(rpmspec -q --queryformat "[%{NAME}\n]" "$specfile" | head -n 1)
	CURRENT_GROUP_NAME=$(rpmspec -q --queryformat "[%{GROUP}\n]" "$specfile" | head -n 1)
}

get_spec_buildrequires() {
	local spec_file_path="$1"
	local build_requires_list=""
	local dependency_token=""

	while read -r dependency_token; do
		[[ -z "$dependency_token" ]] && continue

		local package_name="$dependency_token"

		if [[ "$dependency_token" =~ pkgconfig\((.*)\) ]]; then
			package_name="${BASH_REMATCH[1]}"
		fi

		local package_group="$UNSPECIFIED"
		local rpm_group_info=""

		rpm_group_info=$(rpm -qi "$package_name" 2>/dev/null | grep "Group")

		if [[ "$rpm_group_info" =~ ^Group\ *:\ *(.*)$ ]]; then
			package_group="${BASH_REMATCH[1]}"
		fi

		build_requires_list+="${dependency_token}@${package_group}${IFS}"

	done <<< $(rpmspec -q --buildrequires "$spec_file_path")

	echo "$build_requires_list"
}

get_spec_buildconflicts() {
	local spec_file_path="$1"
	local build_conflicts_list=""
	local conflict_item=""

	while read -r conflict_item; do
		[[ -z "$conflict_item" ]] && continue

		build_conflicts_list+="${conflict_item}@${UNSPECIFIED}${IFS}"
	done <<< $(rpmspec -q --buildconflicts "$spec_file_path")

	echo "$build_conflicts_list"
}

get_spec_requires() {
	local spec_file_path="$1"
	local requires_list=""
	local required_item=""

	while read -r required_item; do
		[[ -z "$required_item" ]] && continue

		requires_list+="${required_item}@${UNSPECIFIED}${IFS}"
	done <<< $(rpmspec -q --queryformat "[%{REQUIRES}\n]" "$spec_file_path")

	echo "$requires_list"
}

run_checker_for_config() {
	local conf_file=$1

	# Config variable reset for checking
	reset_config_variables

	# Get build bit (32bit or 64bit)
	get_build_arch_type

	# Parse config file and check if we should skip
	if parse_config_and_check_skip_condition "$conf_file"; then
		print_message "NOTICE: Config $(basename "$conf_file") skipped"
		return 0
	fi

	# Check if this package is exclueded target
	if [[ -n "$EXCLUDED_TARGET_PACKAGE_NAME" ]]; then
		if [[ $(check_matching "$CURRENT_PACKAGE_NAME" "$EXCLUDED_TARGET_PACKAGE_NAME") == 1 ]]; then
			print_message "NOTICE: $CURRENT_PACKAGE_NAME is excluded by config $(basename "$conf_file")"
			return 0
		fi
	fi

	# Check if this package meets the rule inspection criteria
	if check_target_criteria; then
		echo "Matched Config: $(basename "$conf_file") -> Running checks..."
		if ! check_dependencies_against_config; then
			return 1 # Error occurred
		fi

		if ! check_allowed_files_against_config; then
			return 1 # Error occurred
		fi

		if ! check_denied_ldd_against_config; then
			return 1 # Error occurred
		fi
	fi

	return 0
}

main() {
	SOURCEDIR=$1
	RPM_BUILD_ROOT=$2
	BUILD_ARCH=$3

	# 1. Get specfile from current build package
	if ! CURRENT_PACKAGE_SPECFILE=$(get_current_package_specfile "$SOURCEDIR"); then
		echo "Cannot locate specfile."
		exit 0
	fi

	# 2. Get package name information from acquired specfile step 1.
	get_current_package_metadata "$CURRENT_PACKAGE_SPECFILE"
	echo "Package: $CURRENT_PACKAGE_NAME, Group: $CURRENT_GROUP_NAME"

	# 3. Get all config files(all rule yaml files)
	local config_files_str
	config_files_str=$(get_all_config_files)

	if [[ -z "$config_files_str" ]]; then
		echo "No configuration files found in $ROOTSTRAP_DEFAULT_RULES_DIR"
		exit 0
	fi

	# 4. Inspect all yaml files based on target criteria
	local has_error=0
	local config_file
	for config_file in $config_files_str; do
		if ! run_checker_for_config "$config_file"; then
			has_error=1
			# If package violates the rule, checker stops immediately.
			break
		fi
	done

	if [[ $has_error == 1 ]]; then
		echo "FAILURE: Violation detected."
		exit 1
	fi

	print_border
	print_message "SUCCESS: app-rootstrap-checker done"
	print_border
}

main "$@"
