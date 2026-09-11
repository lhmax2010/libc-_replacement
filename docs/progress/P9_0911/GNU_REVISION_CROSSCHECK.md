# GNU 13.3 与平台 14.2 声明对照 / Declaration crosscheck

主文源码以固定平台 14.2 行号为准。这里逐段比对现存宿主 13.3 头；SAME_EXCERPT 表示字节完全一致，CHANGED_EXCERPT 表示版本片段不同并同时展示实际片段。全头不宣称相同。历史探针头的哈希 NOT_AVAILABLE。

Main excerpts use platform GCC 14.2 line numbers. SAME_EXCERPT means exact equality with an excerpt in the currently available host 13.3 header. CHANGED_EXCERPT shows both actual fragments. Whole-file or historical byte identity is not claimed.

## gany: std/any:83–99 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/any.txt#L83) ↔ [GCC 13.3](sources/gnu13/std/any.txt#L82) (82–98)

GCC 13.3:

```cpp
    // Holds either pointer to a heap object or the contained object itself.
    union _Storage
    {
      constexpr _Storage() : _M_ptr{nullptr} {}

      // Prevent trivial copies of this type, buffer might hold a non-POD.
      _Storage(const _Storage&) = delete;
      _Storage& operator=(const _Storage&) = delete;

      void* _M_ptr;
      aligned_storage<sizeof(_M_ptr), alignof(void*)>::type _M_buffer;
    };

    template<typename _Tp, typename _Safe = is_nothrow_move_constructible<_Tp>,
	     bool _Fits = (sizeof(_Tp) <= sizeof(_Storage))
			  && (alignof(_Tp) <= alignof(_Storage))>
      using _Internal = std::integral_constant<bool, _Safe::value && _Fits>;
```

## gany: std/any:361–362 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/any.txt#L361) ↔ [GCC 13.3](sources/gnu13/std/any.txt#L360) (360–361)

GCC 13.3:

```cpp
    void (*_M_manager)(_Op, const any*, _Arg*);
    _Storage _M_storage;
```

## gany: std/any:401–424 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/any.txt#L401) ↔ [GCC 13.3](sources/gnu13/std/any.txt#L400) (400–423)

GCC 13.3:

```cpp
    // Manage external contained object.
    template<typename _Tp>
      struct _Manager_external
      {
	static void
	_S_manage(_Op __which, const any* __anyp, _Arg* __arg);

	template<typename _Up>
	  static void
	  _S_create(_Storage& __storage, _Up&& __value)
	  {
	    __storage._M_ptr = new _Tp(std::forward<_Up>(__value));
	  }
	template<typename... _Args>
	  static void
	  _S_create(_Storage& __storage, _Args&&... __args)
	  {
	    __storage._M_ptr = new _Tp(std::forward<_Args>(__args)...);
	  }
	static _Tp*
	_S_access(const _Storage& __storage)
	{
	  // The contained object is in *__storage._M_ptr
	  return static_cast<_Tp*>(__storage._M_ptr);
```

## gfun: bits/std_function.h:62–100 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/std_function.h.txt#L62) ↔ [GCC 13.3](sources/gnu13/bits/std_function.h.txt#L62) (62–100)

GCC 13.3:

```cpp
  /**
   *  Trait identifying "location-invariant" types, meaning that the
   *  address of the object (or any of its members) will not escape.
   *  Trivially copyable types are location-invariant and users can
   *  specialize this trait for other types.
   */
  template<typename _Tp>
    struct __is_location_invariant
    : is_trivially_copyable<_Tp>::type
    { };

  class _Undefined_class;

  union _Nocopy_types
  {
    void*       _M_object;
    const void* _M_const_object;
    void (*_M_function_pointer)();
    void (_Undefined_class::*_M_member_pointer)();
  };

  union [[gnu::may_alias]] _Any_data
  {
    void*       _M_access()       noexcept { return &_M_pod_data[0]; }
    const void* _M_access() const noexcept { return &_M_pod_data[0]; }

    template<typename _Tp>
      _Tp&
      _M_access() noexcept
      { return *static_cast<_Tp*>(_M_access()); }

    template<typename _Tp>
      const _Tp&
      _M_access() const noexcept
      { return *static_cast<const _Tp*>(_M_access()); }

    _Nocopy_types _M_unused;
    char _M_pod_data[sizeof(_Nocopy_types)];
  };
```

## gfun: bits/std_function.h:117–128 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/std_function.h.txt#L117) ↔ [GCC 13.3](sources/gnu13/bits/std_function.h.txt#L117) (117–128)

GCC 13.3:

```cpp
    static const size_t _M_max_size = sizeof(_Nocopy_types);
    static const size_t _M_max_align = __alignof__(_Nocopy_types);

    template<typename _Functor>
      class _Base_manager
      {
      protected:
	static const bool __stored_locally =
	(__is_location_invariant<_Functor>::value
	 && sizeof(_Functor) <= _M_max_size
	 && __alignof__(_Functor) <= _M_max_align
	 && (_M_max_align % __alignof__(_Functor) == 0));
```

## gfun: bits/std_function.h:249–254 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/std_function.h.txt#L249) ↔ [GCC 13.3](sources/gnu13/bits/std_function.h.txt#L249) (249–254)

GCC 13.3:

```cpp
    using _Manager_type
      = bool (*)(_Any_data&, const _Any_data&, _Manager_operation);

    _Any_data     _M_functor{};
    _Manager_type _M_manager{};
  };
```

## gfun: bits/std_function.h:333–336 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/std_function.h.txt#L333) ↔ [GCC 13.3](sources/gnu13/bits/std_function.h.txt#L333) (333–336)

GCC 13.3:

```cpp
  template<typename _Res, typename... _ArgTypes>
    class function<_Res(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
      private _Function_base
```

## gfun: bits/std_function.h:666–668 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/std_function.h.txt#L666) ↔ [GCC 13.3](sources/gnu13/bits/std_function.h.txt#L666) (666–668)

GCC 13.3:

```cpp
    private:
      using _Invoker_type = _Res (*)(const _Any_data&, _ArgTypes&&...);
      _Invoker_type _M_invoker = nullptr;
```

## gdeq: bits/stl_deque.h:83–98 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_deque.h.txt#L83) ↔ [GCC 13.3](sources/gnu13/bits/stl_deque.h.txt#L83) (83–98)

GCC 13.3:

```cpp
   *  seems to be a useful wrapper around a repeated constant
   *  expression.  The @b 512 is tunable (and no other code needs to
   *  change), but no investigation has been done since inheriting the
   *  SGI code.  Touch _GLIBCXX_DEQUE_BUF_SIZE only if you know what
   *  you are doing, however: changing it breaks the binary
   *  compatibility!!
  */

#ifndef _GLIBCXX_DEQUE_BUF_SIZE
#define _GLIBCXX_DEQUE_BUF_SIZE 512
#endif

  _GLIBCXX_CONSTEXPR inline size_t
  __deque_buf_size(size_t __size)
  { return (__size < _GLIBCXX_DEQUE_BUF_SIZE
	    ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1)); }
```

## gdeq: bits/stl_deque.h:142–145 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_deque.h.txt#L142) ↔ [GCC 13.3](sources/gnu13/bits/stl_deque.h.txt#L142) (142–145)

GCC 13.3:

```cpp
      _Elt_pointer _M_cur;
      _Elt_pointer _M_first;
      _Elt_pointer _M_last;
      _Map_pointer _M_node;
```

## gdeq: bits/stl_deque.h:509–515 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_deque.h.txt#L509) ↔ [GCC 13.3](sources/gnu13/bits/stl_deque.h.txt#L509) (509–515)

GCC 13.3:

```cpp
      struct _Deque_impl_data
      {
	_Map_pointer _M_map;
	size_t _M_map_size;
	iterator _M_start;
	iterator _M_finish;
```

## gdeq: bits/stl_deque.h:539–544 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_deque.h.txt#L539) ↔ [GCC 13.3](sources/gnu13/bits/stl_deque.h.txt#L539) (539–544)

GCC 13.3:

```cpp
      // This struct encapsulates the implementation of the std::deque
      // standard container and at the same time makes use of the EBO
      // for empty allocators.
      struct _Deque_impl
      : public _Tp_alloc_type, public _Deque_impl_data
      {
```

## gdeqwhy: bits/stl_deque.h:722–740 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_deque.h.txt#L722) ↔ [GCC 13.3](sources/gnu13/bits/stl_deque.h.txt#L722) (722–740)

GCC 13.3:

```cpp
   *  Here's how a deque<Tp> manages memory.  Each deque has 4 members:
   *
   *  - Tp**        _M_map
   *  - size_t      _M_map_size
   *  - iterator    _M_start, _M_finish
   *
   *  map_size is at least 8.  %map is an array of map_size
   *  pointers-to-@a nodes.  (The name %map has nothing to do with the
   *  std::map class, and @b nodes should not be confused with
   *  std::list's usage of @a node.)
   *
   *  A @a node has no specific type name as such, but it is referred
   *  to as @a node in this file.  It is a simple array-of-Tp.  If Tp
   *  is very large, there will be one Tp element per node (i.e., an
   *  @a array of one).  For non-huge Tp's, node size is inversely
   *  related to Tp size: the larger the Tp, the fewer Tp's will fit
   *  in a node.  The goal here is to keep the total size of a node
   *  relatively small and constant over different Tp's, to improve
   *  allocator efficiency.
```

## gdeqwhy: bits/stl_deque.h:750–763 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_deque.h.txt#L750) ↔ [GCC 13.3](sources/gnu13/bits/stl_deque.h.txt#L750) (750–763)

GCC 13.3:

```cpp
   *  Class invariants:
   * - For any nonsingular iterator i:
   *    - i.node points to a member of the %map array.  (Yes, you read that
   *      correctly:  i.node does not actually point to a node.)  The member of
   *      the %map array is what actually points to the node.
   *    - i.first == *(i.node)    (This points to the node (first Tp element).)
   *    - i.last  == i.first + node_size
   *    - i.cur is a pointer in the range [i.first, i.last).  NOTE:
   *      the implication of this is that i.cur is always a dereferenceable
   *      pointer, even if i is a past-the-end iterator.
   * - Start and Finish are always nonsingular iterators.  NOTE: this
   * means that an empty deque must have one node, a deque with <N
   * elements (where N is the node buffer size) must have one node, a
   * deque with N through (2N-1) elements must have two nodes, etc.
```

## gmap: bits/stl_map.h:149–158 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_map.h.txt#L149) ↔ [GCC 13.3](sources/gnu13/bits/stl_map.h.txt#L149) (149–158)

GCC 13.3:

```cpp
    private:
      /// This turns a red-black tree into a [multi]map.
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<value_type>::other _Pair_alloc_type;

      typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
		       key_compare, _Pair_alloc_type> _Rep_type;

      /// The actual tree structure.
      _Rep_type _M_t;
```

## gset: bits/stl_set.h:129–135 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_set.h.txt#L129) ↔ [GCC 13.3](sources/gnu13/bits/stl_set.h.txt#L129) (129–135)

GCC 13.3:

```cpp
    private:
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<_Key>::other _Key_alloc_type;

      typedef _Rb_tree<key_type, value_type, _Identity<value_type>,
		       key_compare, _Key_alloc_type> _Rep_type;
      _Rep_type _M_t;  // Red-black tree representing set.
```

## gset: bits/stl_set.h:145–150 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_set.h.txt#L145) ↔ [GCC 13.3](sources/gnu13/bits/stl_set.h.txt#L145) (145–150)

GCC 13.3:

```cpp
      typedef typename _Alloc_traits::const_reference	 const_reference;
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 103. set::iterator is required to be modifiable,
      // but this allows modification of keys.
      typedef typename _Rep_type::const_iterator	 iterator;
      typedef typename _Rep_type::const_iterator	 const_iterator;
```

## gtree: bits/stl_tree.h:95–105 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L95) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L99) (99–109)

GCC 13.3:

```cpp
  enum _Rb_tree_color { _S_red = false, _S_black = true };

  struct _Rb_tree_node_base
  {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color	_M_color;
    _Base_ptr		_M_parent;
    _Base_ptr		_M_left;
    _Base_ptr		_M_right;
```

## gtree: bits/stl_tree.h:136–140 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L136) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L140) (140–144)

GCC 13.3:

```cpp
  // Helper type offering value initialization guarantee on the compare functor.
  template<typename _Key_compare>
    struct _Rb_tree_key_compare
    {
      _Key_compare		_M_key_compare;
```

## gtree: bits/stl_tree.h:164–167 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L164) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L168) (168–171)

GCC 13.3:

```cpp
  struct _Rb_tree_header
  {
    _Rb_tree_node_base	_M_header;
    size_t		_M_node_count; // Keeps track of size of tree.
```

## gtree: bits/stl_tree.h:658–661 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L658) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L662) (662–665)

GCC 13.3:

```cpp
	struct _Rb_tree_impl
	: public _Node_allocator
	, public _Rb_tree_key_compare<_Key_compare>
	, public _Rb_tree_header
```

## gtree: bits/stl_tree.h:704–704 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L704) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L708) (708–708)

GCC 13.3:

```cpp
      _Rb_tree_impl<_Compare> _M_impl;
```

## gtreewhy: bits/stl_tree.h:79–93 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L79) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L83) (83–97)

GCC 13.3:

```cpp
  // Red-black tree class, designed for use in implementing STL
  // associative containers (set, multiset, map, and multimap). The
  // insertion and deletion algorithms are based on those in Cormen,
  // Leiserson, and Rivest, Introduction to Algorithms (MIT Press,
  // 1990), except that
  //
  // (1) the header cell is maintained with links not only to the root
  // but also to the leftmost node of the tree, to enable constant
  // time begin(), and to the rightmost node of the tree, to enable
  // linear time performance when used with the generic set algorithms
  // (set_union, etc.)
  //
  // (2) when a node being deleted has two children its successor node
  // is relinked into its place, rather than copied, so that the only
  // iterators invalidated are those referring to the deleted node.
```

## gnode: bits/stl_tree.h:211–214 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L211) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L215) (215–218)

GCC 13.3:

```cpp
  template<typename _Val>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
      typedef _Rb_tree_node<_Val>* _Link_type;
```

## gnode: bits/stl_tree.h:226–227 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_tree.h.txt#L226) ↔ [GCC 13.3](sources/gnu13/bits/stl_tree.h.txt#L230) (230–231)

GCC 13.3:

```cpp
#else
      __gnu_cxx::__aligned_membuf<_Val> _M_storage;
```

## gumap: bits/unordered_map.h:43–58 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/unordered_map.h.txt#L43) ↔ [GCC 13.3](sources/gnu13/bits/unordered_map.h.txt#L43) (43–58)

GCC 13.3:

```cpp
  /// Base types for unordered_map.
  template<bool _Cache>
    using __umap_traits = __detail::_Hashtable_traits<_Cache, false, true>;

  template<typename _Key,
	   typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = std::equal_to<_Key>,
	   typename _Alloc = std::allocator<std::pair<const _Key, _Tp> >,
	   typename _Tr = __umap_traits<__cache_default<_Key, _Hash>::value>>
    using __umap_hashtable = _Hashtable<_Key, std::pair<const _Key, _Tp>,
                                        _Alloc, __detail::_Select1st,
				        _Pred, _Hash,
				        __detail::_Mod_range_hashing,
				        __detail::_Default_ranged_hash,
				        __detail::_Prime_rehash_policy, _Tr>;
```

## gumap: bits/unordered_map.h:105–112 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/unordered_map.h.txt#L105) ↔ [GCC 13.3](sources/gnu13/bits/unordered_map.h.txt#L105) (105–112)

GCC 13.3:

```cpp
  template<typename _Key, typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = equal_to<_Key>,
	   typename _Alloc = allocator<std::pair<const _Key, _Tp>>>
    class unordered_map
    {
      typedef __umap_hashtable<_Key, _Tp, _Hash, _Pred, _Alloc>  _Hashtable;
      _Hashtable _M_h;
```

## guset: bits/unordered_set.h:43–56 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/unordered_set.h.txt#L43) ↔ [GCC 13.3](sources/gnu13/bits/unordered_set.h.txt#L43) (43–56)

GCC 13.3:

```cpp
  /// Base types for unordered_set.
  template<bool _Cache>
    using __uset_traits = __detail::_Hashtable_traits<_Cache, true, true>;

  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = std::equal_to<_Value>,
  	   typename _Alloc = std::allocator<_Value>,
	   typename _Tr = __uset_traits<__cache_default<_Value, _Hash>::value>>
    using __uset_hashtable = _Hashtable<_Value, _Value, _Alloc,
					__detail::_Identity, _Pred, _Hash,
					__detail::_Mod_range_hashing,
					__detail::_Default_ranged_hash,
					__detail::_Prime_rehash_policy, _Tr>;
```

## guset: bits/unordered_set.h:100–107 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/unordered_set.h.txt#L100) ↔ [GCC 13.3](sources/gnu13/bits/unordered_set.h.txt#L100) (100–107)

GCC 13.3:

```cpp
  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = equal_to<_Value>,
	   typename _Alloc = allocator<_Value>>
    class unordered_set
    {
      typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
      _Hashtable _M_h;
```

## ghash: bits/hashtable.h:47–52 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable.h.txt#L47) ↔ [GCC 13.3](sources/gnu13/bits/hashtable.h.txt#L47) (47–52)

GCC 13.3:

```cpp
  template<typename _Tp, typename _Hash>
    using __cache_default
      =  __not_<__and_<// Do not cache for fast hasher.
		       __is_fast_hash<_Hash>,
		       // Mandatory to have erase not throwing.
		       __is_nothrow_invocable<const _Hash&, const _Tp&>>>;
```

## ghash: bits/hashtable.h:391–404 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable.h.txt#L391) ↔ [GCC 13.3](sources/gnu13/bits/hashtable.h.txt#L386) (386–399)

GCC 13.3:

```cpp
    private:
      __buckets_ptr		_M_buckets		= &_M_single_bucket;
      size_type			_M_bucket_count		= 1;
      __node_base		_M_before_begin;
      size_type			_M_element_count	= 0;
      _RehashPolicy		_M_rehash_policy;

      // A single bucket used when only need for 1 bucket. Especially
      // interesting in move semantic to leave hashtable with only 1 bucket
      // which is not allocated so that we can have those operations noexcept
      // qualified.
      // Note that we can't leave hashtable with 0 bucket without adding
      // numerous checks in the code to avoid 0 modulus.
      __node_base_ptr		_M_single_bucket	= nullptr;
```

## ghashwhy: bits/hashtable.h:128–146 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable.h.txt#L128) ↔ [GCC 13.3](sources/gnu13/bits/hashtable.h.txt#L128) (128–146)

GCC 13.3:

```cpp
   *  The non-empty buckets contain the node before the first node in the
   *  bucket. This design makes it possible to implement something like a
   *  std::forward_list::insert_after on container insertion and
   *  std::forward_list::erase_after on container erase
   *  calls. _M_before_begin is equivalent to
   *  std::forward_list::before_begin. Empty buckets contain
   *  nullptr.  Note that one of the non-empty buckets contains
   *  &_M_before_begin which is not a dereferenceable node so the
   *  node pointer in a bucket shall never be dereferenced, only its
   *  next node can be.
   *
   *  Walking through a bucket's nodes requires a check on the hash code to
   *  see if each node is still in the bucket. Such a design assumes a
   *  quite efficient hash functor and is one of the reasons it is
   *  highly advisable to set __cache_hash_code to true.
   *
   *  The container iterators are simply built from nodes. This way
   *  incrementing the iterator is perfectly efficient independent of
   *  how many empty buckets there are in the container.
```

## gpolicy: bits/hashtable_policy.h:315–318 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable_policy.h.txt#L315) ↔ [GCC 13.3](sources/gnu13/bits/hashtable_policy.h.txt#L309) (309–312)

GCC 13.3:

```cpp
  struct _Hash_node_base
  {
    _Hash_node_base* _M_nxt;
```

## gpolicy: bits/hashtable_policy.h:329–334 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable_policy.h.txt#L329) ↔ [GCC 13.3](sources/gnu13/bits/hashtable_policy.h.txt#L323) (323–328)

GCC 13.3:

```cpp
  template<typename _Value>
    struct _Hash_node_value_base
    {
      typedef _Value value_type;

      __gnu_cxx::__aligned_buffer<_Value> _M_storage;
```

## gpolicy: bits/hashtable_policy.h:360–384 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable_policy.h.txt#L360) ↔ [GCC 13.3](sources/gnu13/bits/hashtable_policy.h.txt#L354) (354–378)

GCC 13.3:

```cpp
  template<bool _Cache_hash_code>
    struct _Hash_node_code_cache
    { };

  /**
   *  Specialization for node with cache, struct _Hash_node_code_cache.
   */
  template<>
    struct _Hash_node_code_cache<true>
    { std::size_t  _M_hash_code; };

  template<typename _Value, bool _Cache_hash_code>
    struct _Hash_node_value
    : _Hash_node_value_base<_Value>
    , _Hash_node_code_cache<_Cache_hash_code>
    { };

  /**
   *  Primary template struct _Hash_node.
   */
  template<typename _Value, bool _Cache_hash_code>
    struct _Hash_node
    : _Hash_node_base
    , _Hash_node_value<_Value, _Cache_hash_code>
    {
```

## gpolicy: bits/hashtable_policy.h:544–551 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable_policy.h.txt#L544) ↔ [GCC 13.3](sources/gnu13/bits/hashtable_policy.h.txt#L538) (538–545)

GCC 13.3:

```cpp
  /// Default value for rehash policy.  Bucket size is (usually) the
  /// smallest prime that keeps the load factor small enough.
  struct _Prime_rehash_policy
  {
    using __has_load_factor = true_type;

    _Prime_rehash_policy(float __z = 1.0) noexcept
    : _M_max_load_factor(__z), _M_next_resize(0) { }
```

## gpolicy: bits/hashtable_policy.h:588–592 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/hashtable_policy.h.txt#L588) ↔ [GCC 13.3](sources/gnu13/bits/hashtable_policy.h.txt#L582) (582–586)

GCC 13.3:

```cpp
    static const std::size_t _S_growth_factor = 2;

    float		_M_max_load_factor;
    mutable std::size_t	_M_next_resize;
  };
```

## gcache: bits/functional_hash.h:114–122 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/functional_hash.h.txt#L114) ↔ [GCC 13.3](sources/gnu13/bits/functional_hash.h.txt#L114) (114–122)

GCC 13.3:

```cpp
  // Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp) 	\
  template<>						\
    struct hash<_Tp> : public __hash_base<size_t, _Tp>  \
    {                                                   \
      size_t                                            \
      operator()(_Tp __val) const noexcept              \
      { return static_cast<size_t>(__val); }            \
    };
```

## gcache: bits/functional_hash.h:153–154 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/functional_hash.h.txt#L153) ↔ [GCC 13.3](sources/gnu13/bits/functional_hash.h.txt#L153) (153–154)

GCC 13.3:

```cpp
  /// Explicit specialization for int.
  _Cxx_hashtable_define_trivial_hash(int)
```

## gcache: bits/functional_hash.h:286–299 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/functional_hash.h.txt#L286) ↔ [GCC 13.3](sources/gnu13/bits/functional_hash.h.txt#L286) (286–299)

GCC 13.3:

```cpp
   * will cache the hash code.
   * The default behavior is to consider that hashers are fast unless specified
   * otherwise.
   *
   * Users can specialize this for their own hash functions in order to force
   * caching of hash codes in unordered containers. Specializing this trait
   * affects the ABI of the unordered containers, so use it carefully.
   */
  template<typename _Hash>
    struct __is_fast_hash : public std::true_type
    { };

  template<>
    struct __is_fast_hash<hash<long double>> : public std::false_type
```

## gstr: bits/basic_string.h:185–186 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/basic_string.h.txt#L185) ↔ [GCC 13.3](sources/gnu13/bits/basic_string.h.txt#L180) (180–181)

GCC 13.3:

```cpp
      // Use empty-base optimization: http://www.cantrip.org/emptyopt.html
      struct _Alloc_hider : allocator_type // TODO check __is_final
```

## gstr: bits/basic_string.h:201–213 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/basic_string.h.txt#L201) ↔ [GCC 13.3](sources/gnu13/bits/basic_string.h.txt#L196) (196–208)

GCC 13.3:

```cpp
	pointer _M_p; // The actual data.
      };

      _Alloc_hider	_M_dataplus;
      size_type		_M_string_length;

      enum { _S_local_capacity = 15 / sizeof(_CharT) };

      union
      {
	_CharT           _M_local_buf[_S_local_capacity + 1];
	size_type        _M_allocated_capacity;
      };
```

## gstr: bits/basic_string.h:265–276 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/basic_string.h.txt#L265) ↔ [GCC 13.3](sources/gnu13/bits/basic_string.h.txt#L260) (260–271)

GCC 13.3:

```cpp
      _GLIBCXX20_CONSTEXPR
      bool
      _M_is_local() const
      {
	if (_M_data() == _M_local_data())
	  {
	    if (_M_string_length > _S_local_capacity)
	      __builtin_unreachable();
	    return true;
	  }
	return false;
      }
```

## gfuture: std/future:700–709 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L700) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L693) (693–702)

GCC 13.3:

```cpp
  /// Common implementation for future and shared_future.
  template<typename _Res>
    class __basic_future : public __future_base
    {
    protected:
      typedef shared_ptr<_State_base>		__state_type;
      typedef __future_base::_Result<_Res>&	__result_type;

    private:
      __state_type 		_M_state;
```

## gfuture: std/future:790–808 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L790) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L783) (783–801)

GCC 13.3:

```cpp
  /// Primary template for future.
  template<typename _Res>
    class future : public __basic_future<_Res>
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 3458. Is shared_future intended to work with arrays or function types?
      static_assert(!is_array<_Res>{}, "result type must not be an array");
      static_assert(!is_function<_Res>{}, "result type must not be a function");
      static_assert(is_destructible<_Res>{},
		    "result type must be destructible");

      friend class promise<_Res>;
      template<typename> friend class packaged_task;
      template<typename _Fn, typename... _Args>
        friend future<__async_result_of<_Fn, _Args...>>
        async(launch, _Fn&&, _Args&&...);

      typedef __basic_future<_Res> _Base_type;
      typedef typename _Base_type::__state_type __state_type;
```

## gshared: bits/shared_ptr_base.h:1135–1136 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/shared_ptr_base.h.txt#L1135) ↔ [GCC 13.3](sources/gnu13/bits/shared_ptr_base.h.txt#L1134) (1134–1135)

GCC 13.3:

```cpp

      _Sp_counted_base<_Lp>*  _M_pi;
```

## gshared: bits/shared_ptr_base.h:1789–1790 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/shared_ptr_base.h.txt#L1789) ↔ [GCC 13.3](sources/gnu13/bits/shared_ptr_base.h.txt#L1785) (1785–1786)

GCC 13.3:

```cpp
      element_type*	   _M_ptr;         // Contained pointer.
      __shared_count<_Lp>  _M_refcount;    // Reference counter.
```

## gshared: bits/shared_ptr_base.h:232–238 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/shared_ptr_base.h.txt#L232) ↔ [GCC 13.3](sources/gnu13/bits/shared_ptr_base.h.txt#L232) (232–238)

GCC 13.3:

```cpp

    private:
      _Sp_counted_base(_Sp_counted_base const&) = delete;
      _Sp_counted_base& operator=(_Sp_counted_base const&) = delete;

      _Atomic_word  _M_use_count;     // #shared
      _Atomic_word  _M_weak_count;    // #weak + (#shared != 0)
```

## gstate: std/future:224–246 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L224) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L217) (217–239)

GCC 13.3:

```cpp
    struct _Result_base
    {
      exception_ptr		_M_error;

      _Result_base(const _Result_base&) = delete;
      _Result_base& operator=(const _Result_base&) = delete;

      // _M_destroy() allows derived classes to control deallocation
      virtual void _M_destroy() = 0;

      struct _Deleter
      {
	void operator()(_Result_base* __fr) const { __fr->_M_destroy(); }
      };

    protected:
      _Result_base();
      virtual ~_Result_base();
    };

    /// A unique_ptr for result objects.
    template<typename _Res>
      using _Ptr = unique_ptr<_Res, _Result_base::_Deleter>;
```

## gstate: std/future:248–254 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L248) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L241) (241–247)

GCC 13.3:

```cpp
    /// A result object that has storage for an object of type _Res.
    template<typename _Res>
      struct _Result : _Result_base
      {
      private:
	__gnu_cxx::__aligned_buffer<_Res>	_M_storage;
	bool 					_M_initialized;
```

## gstate: std/future:329–351 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L329) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L322) (322–344)

GCC 13.3:

```cpp
    // Base class for various types of shared state created by an
    // asynchronous provider (such as a std::promise) and shared with one
    // or more associated futures.
    class _State_baseV2
    {
      typedef _Ptr<_Result_base> _Ptr_type;

      enum _Status : unsigned {
	__not_ready,
	__ready
      };

      _Ptr_type			_M_result;
      __atomic_futex_unsigned<>	_M_status;
      atomic_flag         	_M_retrieved = ATOMIC_FLAG_INIT;
      once_flag			_M_once;

    public:
      _State_baseV2() noexcept : _M_result(), _M_status(_Status::__not_ready)
	{ }
      _State_baseV2(const _State_baseV2&) = delete;
      _State_baseV2& operator=(const _State_baseV2&) = delete;
      virtual ~_State_baseV2() = default;
```

## gfutex: bits/atomic_futex.h:50–52 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/atomic_futex.h.txt#L50) ↔ [GCC 13.3](sources/gnu13/bits/atomic_futex.h.txt#L50) (50–52)

GCC 13.3:

```cpp
#ifdef _GLIBCXX_HAS_GTHREADS
#if defined(_GLIBCXX_HAVE_LINUX_FUTEX) && ATOMIC_INT_LOCK_FREE > 1
  struct __atomic_futex_unsigned_base
```

## gfutex: bits/atomic_futex.h:70–76 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/atomic_futex.h.txt#L70) ↔ [GCC 13.3](sources/gnu13/bits/atomic_futex.h.txt#L70) (70–76)

GCC 13.3:

```cpp
  template <unsigned _Waiter_bit = 0x80000000>
  class __atomic_futex_unsigned : __atomic_futex_unsigned_base
  {
    typedef chrono::steady_clock __clock_t;

    // This must be lock-free and at offset 0.
    atomic<unsigned> _M_data;
```

## gstatealg: std/future:427–440 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L427) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L420) (420–433)

GCC 13.3:

```cpp
      // Provide a result to the shared state and make it ready.
      // Calls at most once: _M_result = __res();
      void
      _M_set_result(function<_Ptr_type()> __res, bool __ignore_failure = false)
      {
	bool __did_set = false;
        // all calls to this function are serialized,
        // side-effects of invoking __res only happen once
	call_once(_M_once, &_State_baseV2::_M_do_set, this,
		  std::__addressof(__res), std::__addressof(__did_set));
	if (__did_set)
	  // Use release MO to synchronize with observers of the ready state.
	  _M_status._M_store_notify_all(_Status::__ready,
					memory_order_release);
```

## gstatealg: std/future:484–490 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L484) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L477) (477–483)

GCC 13.3:

```cpp
      // Called when this object is first passed to a future.
      void
      _M_set_retrieved_flag()
      {
	if (_M_retrieved.test_and_set())
	  __throw_future_error(int(future_errc::future_already_retrieved));
      }
```

## gstatealg: std/future:505–510 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L505) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L498) (498–503)

GCC 13.3:

```cpp
	  // Used by std::promise to copy construct the result.
          typename promise<_Res>::_Ptr_type operator()() const
          {
            _M_promise->_M_storage->_M_set(*_M_arg);
            return std::move(_M_promise->_M_storage);
          }
```

## gpromise: std/future:1097–1110 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L1097) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L1090) (1090–1103)

GCC 13.3:

```cpp
      typedef __future_base::_State_base 	_State;
      typedef __future_base::_Result<_Res>	_Res_type;
      typedef __future_base::_Ptr<_Res_type>	_Ptr_type;
      template<typename, typename> friend struct _State::_Setter;
      friend _State;

      shared_ptr<_State>                        _M_future;
      _Ptr_type                                 _M_storage;

    public:
      promise()
      : _M_future(std::make_shared<_State>()),
	_M_storage(new _Res_type())
      { }
```

## gpromise: std/future:1131–1135 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/std/future.txt#L1131) ↔ [GCC 13.3](sources/gnu13/std/future.txt#L1124) (1124–1128)

GCC 13.3:

```cpp
      ~promise()
      {
        if (static_cast<bool>(_M_future) && !_M_future.unique())
          _M_future->_M_break_promise(std::move(_M_storage));
      }
```

## gup: bits/unique_ptr.h:224–227 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/unique_ptr.h.txt#L224) ↔ [GCC 13.3](sources/gnu13/bits/unique_ptr.h.txt#L230) (230–233)

GCC 13.3:

```cpp

    private:
      tuple<pointer, _Dp> _M_t;
    };
```

## gup: bits/unique_ptr.h:272–277 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/unique_ptr.h.txt#L272) ↔ [GCC 13.3](sources/gnu13/bits/unique_ptr.h.txt#L278) (278–283)

GCC 13.3:

```cpp
    {
      template <typename _Up>
	using _DeleterConstraint =
	  typename __uniq_ptr_impl<_Tp, _Up>::_DeleterConstraint::type;

      __uniq_ptr_data<_Tp, _Dp> _M_t;
```

## gless: bits/stl_function.h:130–141 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_function.h.txt#L130) ↔ [GCC 13.3](sources/gnu13/bits/stl_function.h.txt#L130) (130–141)

GCC 13.3:

```cpp
  template<typename _Arg1, typename _Arg2, typename _Result>
    struct binary_function
    {
      /// @c first_argument_type is the type of the first argument
      typedef _Arg1 	first_argument_type; 

      /// @c second_argument_type is the type of the second argument
      typedef _Arg2 	second_argument_type;

      /// @c result_type is the return type
      typedef _Result 	result_type;
    } _GLIBCXX11_DEPRECATED;
```

## gless: bits/stl_function.h:398–406 — SAME_EXCERPT

[GCC 14.2](sources/gnu14/bits/stl_function.h.txt#L398) ↔ [GCC 13.3](sources/gnu13/bits/stl_function.h.txt#L401) (401–409)

GCC 13.3:

```cpp
  /// One of the @link comparison_functors comparison functors@endlink.
  template<typename _Tp>
    struct less : public binary_function<_Tp, _Tp, bool>
    {
      _GLIBCXX14_CONSTEXPR
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x < __y; }
    };
```
