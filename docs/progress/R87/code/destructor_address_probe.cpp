struct W {
  ~W() noexcept = default;
};

auto destructor_address = &W::~W;
