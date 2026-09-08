declare void @external_fct(i32)

define dso_local i32 @top() {
  %a = call i32 @multiplier(i32 5)
  %b = call i32 @adder(i32 10)
  %ret = add nsw i32 %a, %b
  call void @external_fct(i32 %ret)
  ret i32 %ret
}

define internal dso_local i32 @adder(i32 %arg) {
  %slot = alloca i32, align 4
  store i32 %arg, ptr %slot, align 4
  %v1 = load i32, ptr %slot, align 4
  %mul = call i32 @multiplier(i32 %v1)
  %choice = call i32 @switcher(i32 1)
  %sum = add nsw i32 %mul, %choice
  ret i32 %sum
}

define internal i32 @multiplier(i32 %arg) {
  %slot = alloca i32, align 4
  store i32 %arg, ptr %slot, align 4
  %lhs = load i32, ptr %slot, align 4
  %rhs = load i32, ptr %slot, align 4
  %product = mul nsw i32 %lhs, %rhs
  ret i32 %product
}

define i32 @switcher(i32 %arg) {
  %result = alloca i32, align 4
  switch i32 %arg, label %fallback [
    i32 1, label %one
    i32 2, label %two
  ]

one:
  store i32 2, ptr %result, align 4
  br label %done

two:
  %square = call i32 @multiplier(i32 %arg)
  store i32 %square, ptr %result, align 4
  br label %done

fallback:
  %fallback_value = call i32 @adder(i32 2)
  store i32 %fallback_value, ptr %result, align 4
  br label %done

done:
  %value = load i32, ptr %result, align 4
  ret i32 %value
}
