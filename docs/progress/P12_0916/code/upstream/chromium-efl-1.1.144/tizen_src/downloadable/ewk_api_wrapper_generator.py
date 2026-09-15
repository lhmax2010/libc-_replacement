import glob
import sys
import os
import re
from optparse import OptionParser

# Text file containing list of EWK APIs
filepath = 'API_LIST.txt'

API_LIST = []
FUNC_DEFI = []

# ewk_interface_main.cc defines few APIs (like ewk_init, ewk_shutdown etc.) to setup
# and preinitialize dl library parameters. Those APIs are excluded from auto-generated
# code to avoid duplicate symbols.
exclude_list = ["ewk_init", "ewk_shutdown", "ewk_set_version_policy", "ewk_wait_chromium_ready", "ewk_check_chromium_ready"]

# Few ewk headers are directly included in impl, so they are
# manually added here
main_headers_list = ["ewk_interface_main.h", "dlog_util.h"]
headers_list = ["ewk_cookie_parser.h", "ewk_media_playback_info_product.h", "ewk_media_subtitle_info_product.h"]
headers_list_inc = ["EWebKit.h", "EWebKit_internal.h", "EWebKit_product.h", "dlfcn.h"]

basepath = os.path.dirname(os.path.realpath(__file__))
path = basepath + "/../ewk/efl_integration/public/ewk*.h"

files = glob.glob(path)
with open(filepath, 'w') as outfile:
  for file in files:
    if (file.find("ewk_export.h") != -1):
      continue;
    f = open(file, 'r')
    for line in f:
      outfile.write(line)
    f.close()

# Extract lines containing EXPORT_API
api_line = ""
with open(filepath, 'r') as f:
  for line in f:
    if (line.find('EXPORT_API') == -1):
      continue
    else:
      api_line = api_line + line.strip()
      pattern = r'(\);| TIZEN_DEPRECATED_API;\s*)'
      while not re.search(pattern, api_line):
        if sys.version_info < (3, 0):
          api_line = api_line + " " + f.next().strip()
        else:
          api_line = api_line + " " + f.readline().strip()

      # Remove TIZEN_DEPRECATED_API macro
      api_line = api_line.replace(" TIZEN_DEPRECATED_API", "")

      # Special case for ewk_view_hit_test_request, because one of its args does not have name
      if (api_line.find("Ewk_View_Hit_Test_Request_Callback") != -1):
        api_line = api_line.replace("Callback", "Callback callback")

      # Special case for ewk_context_background_music_set to avoid duplicate
      if (api_line.find("ewk_context_background_music_set") != -1):
        api_line = api_line.replace("Ewk_Context *ewkContext", "Ewk_Context* context")

      API_LIST.append(api_line)
      api_line = ""

# Parse function declarations extracted in API_LIST.txt
for line in API_LIST:
  start = line.find('EXPORT_API')
  line = line[start + len('EXPORT_API') + 1:]

  # Parse to extract return type, api name and params
  split_api_params = line.split("(")
  api_name = split_api_params[0]
  index = api_name.rfind(' ')
  RETURN_TYPE = api_name[:index]
  api_name = api_name[index+1:]
  if api_name[0] == '*':
    API_NAME = api_name[1:]
    RETURN_TYPE = RETURN_TYPE + '*'
  else:
    API_NAME = api_name

  # Since ewk_init and ewk_shutdown have manually written code, exclude them here
  should_ignore = False
  for api in exclude_list:
    if (API_NAME == api):
      should_ignore = True

  if (should_ignore):
    continue

  # Generate params list from params string
  params = split_api_params[1]
  index = params.rfind(');')
  PARAMS = params[:index]
  params_list = PARAMS.split(",")

  # Trim '*' char from param name and append it to return type
  idx = 0
  if params_list[0]:
    for x in params_list:
      index = x.rfind(' ')
      x = x[index+1:]
      if (x[0] == '*'):
        x = x[1:]
      if (x == "void"):
        del params_list[idx]
      else:
        params_list[idx] = x
      idx = idx + 1

  # Generate definition code by concatinating string peices and related macro
  func_def = RETURN_TYPE + " " + API_NAME + "(" + PARAMS + ")\n{\n\t"
  func_ptr_decl = "typedef " + RETURN_TYPE + " (*func_ptr_t)(" + PARAMS + ");"

  func_def = func_def + func_ptr_decl + "\n\t"
  func_def = func_def + "DL_FUNCTION("
  func_def = func_def + RETURN_TYPE + ", " + API_NAME

  if len(params_list) > 0:
    for x in params_list:
      func_def = func_def + ', ' + x

  func_def = func_def + ");\n}\n"
  FUNC_DEFI.append(func_def)

  # There might duplicate entries because of inclusion of both public and internal APIs,
  # so make the list unique by creating a set
  FUNC_DEFI = list(set(FUNC_DEFI))

output_file = ""
parser = OptionParser()
parser.add_option('--out-cpp', dest='out_cpp')
(options, args) = parser.parse_args()
output_file = options.out_cpp

# Include required headers
text_file = open(output_file, "w")

for header_file in headers_list_inc:
  str = "#include <" + header_file + ">\n"
  text_file.write(str)
text_file.write("\n")

# Include main headers
for header_file in main_headers_list:
  str = "#include \"" + header_file + "\"\n"
  text_file.write(str)

# Include other ewk headers by checking file existance
header_path = basepath + "/../ewk/efl_integration/public/"
for header_file in headers_list:
  print(header_path + header_file)
  if os.path.isfile(header_path + header_file):
    str = "#include \"" + header_file + "\"\n"
    text_file.write(str)

DL_MACRO = """
#define DL_FUNCTION(return_type, func_name, ...)                            \\
  do {                                                                      \\
    if (!g_impl_lib_handle && !open_library())                              \\
      return static_cast<return_type>(0);                                   \\
    func_ptr_t fp =                                                         \\
        reinterpret_cast<func_ptr_t>(dlsym(g_impl_lib_handle, #func_name)); \\
    if (fp)                                                                 \\
      return fp(__VA_ARGS__);                                               \\
    return static_cast<return_type>(0);                                     \\
  } while (0)
"""

text_file.write(DL_MACRO + "\n")
text_file.write("using namespace ewk_interface;\n\n")

# Write funtion definition to output file
for x in FUNC_DEFI:
  text_file.write(x + '\n')

text_file.close()
