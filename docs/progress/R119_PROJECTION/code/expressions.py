#!/usr/bin/env python3
"""只生成真实命中表达式的检测请求，不填造模板实参。"""
import re
BUILTINS={'void','bool','char','signed','unsigned','short','int','long','float','double','wchar_t','char16_t','char32_t','char8_t','const','volatile','typename','true','false','sizeof','alignof','decltype','nullptr'}
MEMBERS={'value','type','value_type','difference_type','pointer','const_pointer','reference','iterator_category','size_type','max','min','lowest','epsilon','infinity','quiet_NaN','signaling_NaN','denorm_min','is_specialized','digits','digits10','max_digits10','is_signed','is_integer','is_exact','radix','min_exponent','max_exponent','min_exponent10','max_exponent10','has_infinity','has_quiet_NaN','has_signaling_NaN','has_denorm','has_denorm_loss','is_iec559','is_bounded','is_modulo','traps','tinyness_before','round_style'}
TYPE_MEMBERS={'type','value_type','difference_type','pointer','const_pointer','reference','iterator_category','size_type'}
CALL_MEMBERS={'max','min','lowest','epsilon','infinity','quiet_NaN','signaling_NaN','denorm_min'}
def residual_names(expr):
    s=re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',' ',expr,flags=re.S)
    s=re.sub(r'(?:::)?\bstd\s*(?:::\s*[A-Za-z_]\w*)+',' ',s)
    s=re.sub(r'::\s*('+'|'.join(sorted(MEMBERS,key=len,reverse=True))+r')\b',' ',s)
    # Remove numeric literals including exponent/suffix before identifier check.
    s=re.sub(r'\b(?:0[xX][0-9A-Fa-f]+|\d+(?:\.\d*)?(?:[eE][+-]?\d+)?)[uUlLfF]*\b',' ',s)
    return sorted(set(re.findall(r'\b[A-Za-z_]\w*\b',s))-BUILTINS)
def request(h):
    if h.get('foreign_qualification'):return None,'QUALIFIER_NOT_CONFIRMED_GLOBAL_STD'
    if h.get('parse_status')!='BALANCED_LEXICAL_ONLY':return None,h.get('parse_status','NO_ARGUMENTS')
    expr=h['template_expression'];member=h['suffix_member'];following=h['following']
    if '\\' in expr or '#' in expr:return None,'MACRO_CONTINUATION_OR_DIRECTIVE_REQUIRES_REAL_EXPANSION'
    args=h['template_arguments']; name=h['candidate']
    if name in ('std::enable_if','std::enable_if_t') and (member=='type' or name.endswith('_t')) and len(args) in (1,2):
        target=args[1] if len(args)==2 else 'void'
        if not residual_names(target):return target,'CONDITIONAL_ENABLE_IF_PROJECTION: type exists only if the actual condition is true'
    if name in ('std::conditional','std::conditional_t') and (member=='type' or name.endswith('_t')) and len(args)==3:
        if re.sub(r'\s+','',args[1])==re.sub(r'\s+','',args[2]) and not residual_names(args[1]):return args[1],'IDENTICAL_ACTUAL_CONDITIONAL_BRANCHES'
    if name=='std::void_t' and not member:return 'void','VOID_T_DEFAULT_PROJECTION: argument well-formedness not evaluated'
    missing=residual_names(expr)
    if missing:return None,'DEPENDENT_OR_LOCAL_NAMES: '+','.join(missing)
    if len(expr)>12000:return None,'EXPRESSION_LARGE_REQUIRES_CONTEXTUAL_COMPILE' # 未截断；全文仍保存。
    if member in TYPE_MEMBERS:return 'typename '+expr+'::'+member,''
    if member in CALL_MEMBERS:
        if re.match(r'::\s*'+member+r'\s*(?:\)\s*)?\(',following):return 'decltype('+expr+'::'+member+'())',''
        return None,'STATIC_MEMBER_NOT_OBSERVED_AS_ZERO_ARGUMENT_CALL'
    if member in MEMBERS:return 'decltype('+expr+'::'+member+')',''
    if member:return None,'NESTED_MEMBER_REQUIRES_CONTEXT: '+member
    if h['candidate'].endswith('_t'):return expr,''
    return None,'NO_SELECTED_PROJECTION: helper/alias/base/template-name use needs downstream lookup'
