; ModuleID = '/tmp/mlgo_inliner_input.ll'
source_filename = "/tmp/mlgo_inliner_input.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-tizen-linux-gnu"

declare void @external_fct(i32)

define dso_local noundef i32 @top() {
  tail call void @external_fct(i32 127)
  ret i32 127
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i32 2, 7) i32 @switcher(i32 %arg) #0 {
done:
  %switch.selectcmp = icmp eq i32 %arg, 2
  %switch.select = select i1 %switch.selectcmp, i32 4, i32 6
  %switch.selectcmp1 = icmp eq i32 %arg, 1
  %switch.select2 = select i1 %switch.selectcmp1, i32 2, i32 %switch.select
  ret i32 %switch.select2
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #1

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
