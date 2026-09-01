struct base {
  virtual ~base() noexcept = default;
};

struct derived : base {
  ~derived() noexcept(false) {}
};
