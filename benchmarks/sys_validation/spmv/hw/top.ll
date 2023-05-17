; ModuleID = 'top.c'
source_filename = "top.c"
target datalayout = "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-unknown-linux-gnu"

; Function Attrs: nofree noinline norecurse nounwind
define dso_local void @top(i64 %val, i64 %cols, i64 %rowDelims, i64 %vec, i64 %out) local_unnamed_addr #0 {
entry:
  store volatile i64 %val, i64* inttoptr (i32 268566529 to i64*), align 4, !tbaa !3
  store volatile i64 268566720, i64* inttoptr (i32 268566537 to i64*), align 4, !tbaa !3
  store volatile i32 13328, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  %0 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %1 = and i8 %0, 4
  %cmp.not.not = icmp eq i8 %1, 0
  br i1 %cmp.not.not, label %while.cond, label %while.end, !llvm.loop !10

while.end:                                        ; preds = %while.cond
  store volatile i64 %cols, i64* inttoptr (i32 268566529 to i64*), align 4, !tbaa !3
  store volatile i64 268580096, i64* inttoptr (i32 268566537 to i64*), align 4, !tbaa !3
  store volatile i32 6664, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond2

while.cond2:                                      ; preds = %while.cond2, %while.end
  %2 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %3 = and i8 %2, 4
  %cmp5.not.not = icmp eq i8 %3, 0
  br i1 %cmp5.not.not, label %while.cond2, label %while.end8, !llvm.loop !13

while.end8:                                       ; preds = %while.cond2
  store volatile i64 %rowDelims, i64* inttoptr (i32 268566529 to i64*), align 4, !tbaa !3
  store volatile i64 268586816, i64* inttoptr (i32 268566537 to i64*), align 4, !tbaa !3
  store volatile i32 1980, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond9

while.cond9:                                      ; preds = %while.cond9, %while.end8
  %4 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %5 = and i8 %4, 4
  %cmp12.not.not = icmp eq i8 %5, 0
  br i1 %cmp12.not.not, label %while.cond9, label %while.end15, !llvm.loop !14

while.end15:                                      ; preds = %while.cond9
  store volatile i64 %vec, i64* inttoptr (i32 268566529 to i64*), align 4, !tbaa !3
  store volatile i64 268588800, i64* inttoptr (i32 268566537 to i64*), align 4, !tbaa !3
  store volatile i32 3952, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond16

while.cond16:                                     ; preds = %while.cond16, %while.end15
  %6 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %7 = and i8 %6, 4
  %cmp19.not.not = icmp eq i8 %7, 0
  br i1 %cmp19.not.not, label %while.cond16, label %while.end22, !llvm.loop !15

while.end22:                                      ; preds = %while.cond16
  store volatile i8 1, i8* inttoptr (i32 268566656 to i8*), align 128, !tbaa !9
  br label %while.cond23

while.cond23:                                     ; preds = %while.cond23, %while.end22
  %8 = load volatile i8, i8* inttoptr (i32 268566656 to i8*), align 128, !tbaa !9
  %9 = and i8 %8, 4
  %cmp26.not.not = icmp eq i8 %9, 0
  br i1 %cmp26.not.not, label %while.cond23, label %while.end29, !llvm.loop !16

while.end29:                                      ; preds = %while.cond23
  store volatile i64 268592768, i64* inttoptr (i32 268566529 to i64*), align 4, !tbaa !3
  store volatile i64 %out, i64* inttoptr (i32 268566537 to i64*), align 4, !tbaa !3
  store volatile i32 3952, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond30

while.cond30:                                     ; preds = %while.cond30, %while.end29
  %10 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %11 = and i8 %10, 4
  %cmp33.not.not = icmp eq i8 %11, 0
  br i1 %cmp33.not.not, label %while.cond30, label %while.end36, !llvm.loop !17

while.end36:                                      ; preds = %while.cond30
  ret void
}

attributes #0 = { nofree noinline norecurse nounwind "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-builtins" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 12.0.1 (https://github.com/llvm/llvm-project.git fed41342a82f5a3a9201819a82bf7a48313e296b)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"long long", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!8, !8, i64 0}
!8 = !{!"int", !5, i64 0}
!9 = !{!5, !5, i64 0}
!10 = distinct !{!10, !11, !12}
!11 = !{!"llvm.loop.mustprogress"}
!12 = !{!"llvm.loop.unroll.disable"}
!13 = distinct !{!13, !11, !12}
!14 = distinct !{!14, !11, !12}
!15 = distinct !{!15, !11, !12}
!16 = distinct !{!16, !11, !12}
!17 = distinct !{!17, !11, !12}
