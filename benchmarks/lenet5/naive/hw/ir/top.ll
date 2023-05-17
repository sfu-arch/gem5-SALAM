; ModuleID = 'top.c'
source_filename = "top.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

; Function Attrs: nofree norecurse nounwind
define dso_local void @top(i64 %feats, i64 %weights) local_unnamed_addr #0 {
entry:
  store volatile i64 %feats, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268566720, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 4096, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  %0 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %1 = and i8 %0, 4
  %cmp.not.not = icmp eq i8 %1, 0
  br i1 %cmp.not.not, label %while.cond, label %while.end, !llvm.loop !10

while.end:                                        ; preds = %while.cond
  store volatile i64 %weights, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268570880, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 600, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond2

while.cond2:                                      ; preds = %while.cond2, %while.end
  %2 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %3 = and i8 %2, 4
  %cmp5.not.not = icmp eq i8 %3, 0
  br i1 %cmp5.not.not, label %while.cond2, label %while.end8, !llvm.loop !13

while.end8:                                       ; preds = %while.cond2
  store volatile i8 1, i8* inttoptr (i32 268566656 to i8*), align 128, !tbaa !9
  br label %while.cond9

while.cond9:                                      ; preds = %while.cond9, %while.end8
  %4 = load volatile i8, i8* inttoptr (i32 268566656 to i8*), align 128, !tbaa !9
  %5 = and i8 %4, 4
  %cmp12.not.not = icmp eq i8 %5, 0
  br i1 %cmp12.not.not, label %while.cond9, label %while.end15, !llvm.loop !14

while.end15:                                      ; preds = %while.cond9
  store volatile i64 268571520, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 %weights, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 18816, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond16

while.cond16:                                     ; preds = %while.cond16, %while.end15
  %6 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %7 = and i8 %6, 4
  %cmp19.not.not = icmp eq i8 %7, 0
  br i1 %cmp19.not.not, label %while.cond16, label %while.end22, !llvm.loop !15

while.end22:                                      ; preds = %while.cond16
  store volatile i64 2415919104, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268590464, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 18816, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond23

while.cond23:                                     ; preds = %while.cond23, %while.end22
  %8 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %9 = and i8 %8, 4
  %cmp26.not.not = icmp eq i8 %9, 0
  br i1 %cmp26.not.not, label %while.cond23, label %while.end29, !llvm.loop !16

while.end29:                                      ; preds = %while.cond23
  store volatile i8 1, i8* inttoptr (i32 268590400 to i8*), align 64, !tbaa !9
  br label %while.cond30

while.cond30:                                     ; preds = %while.cond30, %while.end29
  %10 = load volatile i8, i8* inttoptr (i32 268590400 to i8*), align 64, !tbaa !9
  %11 = and i8 %10, 4
  %cmp33.not.not = icmp eq i8 %11, 0
  br i1 %cmp33.not.not, label %while.cond30, label %while.end36, !llvm.loop !17

while.end36:                                      ; preds = %while.cond30
  store volatile i64 268609344, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 2415919104, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 4704, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond37

while.cond37:                                     ; preds = %while.cond37, %while.end36
  %12 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %13 = and i8 %12, 4
  %cmp40.not.not = icmp eq i8 %13, 0
  br i1 %cmp40.not.not, label %while.cond37, label %while.end43, !llvm.loop !18

while.end43:                                      ; preds = %while.cond37
  store volatile i64 2415919104, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268614144, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 4704, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond44

while.cond44:                                     ; preds = %while.cond44, %while.end43
  %14 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %15 = and i8 %14, 4
  %cmp47.not.not = icmp eq i8 %15, 0
  br i1 %cmp47.not.not, label %while.cond44, label %while.end50, !llvm.loop !19

while.end50:                                      ; preds = %while.cond44
  store volatile i64 2415919104, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268618880, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 9600, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond51

while.cond51:                                     ; preds = %while.cond51, %while.end50
  %16 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %17 = and i8 %16, 4
  %cmp54.not.not = icmp eq i8 %17, 0
  br i1 %cmp54.not.not, label %while.cond51, label %while.end57, !llvm.loop !20

while.end57:                                      ; preds = %while.cond51
  store volatile i8 1, i8* inttoptr (i32 268614080 to i8*), align 64, !tbaa !9
  br label %while.cond58

while.cond58:                                     ; preds = %while.cond58, %while.end57
  %18 = load volatile i8, i8* inttoptr (i32 268614080 to i8*), align 64, !tbaa !9
  %19 = and i8 %18, 4
  %cmp61.not.not = icmp eq i8 %19, 0
  br i1 %cmp61.not.not, label %while.cond58, label %while.end64, !llvm.loop !21

while.end64:                                      ; preds = %while.cond58
  store volatile i64 268628544, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 2415919104, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 6400, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond65

while.cond65:                                     ; preds = %while.cond65, %while.end64
  %20 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %21 = and i8 %20, 4
  %cmp68.not.not = icmp eq i8 %21, 0
  br i1 %cmp68.not.not, label %while.cond65, label %while.end71, !llvm.loop !22

while.end71:                                      ; preds = %while.cond65
  store volatile i64 2415919104, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268635072, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 6400, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond72

while.cond72:                                     ; preds = %while.cond72, %while.end71
  %22 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %23 = and i8 %22, 4
  %cmp75.not.not = icmp eq i8 %23, 0
  br i1 %cmp75.not.not, label %while.cond72, label %while.end78, !llvm.loop !23

while.end78:                                      ; preds = %while.cond72
  store volatile i8 1, i8* inttoptr (i32 268635008 to i8*), align 128, !tbaa !9
  br label %while.cond79

while.cond79:                                     ; preds = %while.cond79, %while.end78
  %24 = load volatile i8, i8* inttoptr (i32 268635008 to i8*), align 128, !tbaa !9
  %25 = and i8 %24, 4
  %cmp82.not.not = icmp eq i8 %25, 0
  br i1 %cmp82.not.not, label %while.cond79, label %while.end85, !llvm.loop !24

while.end85:                                      ; preds = %while.cond79
  store volatile i64 268641536, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 2415919104, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 1600, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond86

while.cond86:                                     ; preds = %while.cond86, %while.end85
  %26 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %27 = and i8 %26, 4
  %cmp89.not.not = icmp eq i8 %27, 0
  br i1 %cmp89.not.not, label %while.cond86, label %while.end92, !llvm.loop !25

while.end92:                                      ; preds = %while.cond86
  store volatile i64 2415919104, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268643264, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 1600, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond93

while.cond93:                                     ; preds = %while.cond93, %while.end92
  %28 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %29 = and i8 %28, 4
  %cmp96.not.not = icmp eq i8 %29, 0
  br i1 %cmp96.not.not, label %while.cond93, label %while.end99, !llvm.loop !26

while.end99:                                      ; preds = %while.cond93
  store volatile i8 1, i8* inttoptr (i32 268643200 to i8*), align 128, !tbaa !9
  br label %while.cond100

while.cond100:                                    ; preds = %while.cond100, %while.end99
  %30 = load volatile i8, i8* inttoptr (i32 268643200 to i8*), align 128, !tbaa !9
  %31 = and i8 %30, 4
  %cmp103.not.not = icmp eq i8 %31, 0
  br i1 %cmp103.not.not, label %while.cond100, label %while.end106, !llvm.loop !27

while.end106:                                     ; preds = %while.cond100
  store volatile i64 268836992, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 2415919104, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 480, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond107

while.cond107:                                    ; preds = %while.cond107, %while.end106
  %32 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %33 = and i8 %32, 4
  %cmp110.not.not = icmp eq i8 %33, 0
  br i1 %cmp110.not.not, label %while.cond107, label %while.end113, !llvm.loop !28

while.end113:                                     ; preds = %while.cond107
  store volatile i64 2415919104, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 268837568, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 480, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond114

while.cond114:                                    ; preds = %while.cond114, %while.end113
  %34 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %35 = and i8 %34, 4
  %cmp117.not.not = icmp eq i8 %35, 0
  br i1 %cmp117.not.not, label %while.cond114, label %while.end120, !llvm.loop !29

while.end120:                                     ; preds = %while.cond114
  store volatile i8 1, i8* inttoptr (i32 268837504 to i8*), align 128, !tbaa !9
  br label %while.cond121

while.cond121:                                    ; preds = %while.cond121, %while.end120
  %36 = load volatile i8, i8* inttoptr (i32 268837504 to i8*), align 128, !tbaa !9
  %37 = and i8 %36, 4
  %cmp124.not.not = icmp eq i8 %37, 0
  br i1 %cmp124.not.not, label %while.cond121, label %while.end127, !llvm.loop !30

while.end127:                                     ; preds = %while.cond121
  store volatile i64 268878464, i64* inttoptr (i32 268566529 to i64*), align 8, !tbaa !3
  store volatile i64 2415919104, i64* inttoptr (i32 268566537 to i64*), align 8, !tbaa !3
  store volatile i32 336, i32* inttoptr (i32 268566545 to i32*), align 4, !tbaa !7
  store volatile i8 1, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  br label %while.cond128

while.cond128:                                    ; preds = %while.cond128, %while.end127
  %38 = load volatile i8, i8* inttoptr (i32 268566528 to i8*), align 131072, !tbaa !9
  %39 = and i8 %38, 4
  %cmp131.not.not = icmp eq i8 %39, 0
  br i1 %cmp131.not.not, label %while.cond128, label %while.end134, !llvm.loop !31

while.end134:                                     ; preds = %while.cond128
  ret void
}

attributes #0 = { nofree norecurse nounwind "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+strict-align,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
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
!18 = distinct !{!18, !11, !12}
!19 = distinct !{!19, !11, !12}
!20 = distinct !{!20, !11, !12}
!21 = distinct !{!21, !11, !12}
!22 = distinct !{!22, !11, !12}
!23 = distinct !{!23, !11, !12}
!24 = distinct !{!24, !11, !12}
!25 = distinct !{!25, !11, !12}
!26 = distinct !{!26, !11, !12}
!27 = distinct !{!27, !11, !12}
!28 = distinct !{!28, !11, !12}
!29 = distinct !{!29, !11, !12}
!30 = distinct !{!30, !11, !12}
!31 = distinct !{!31, !11, !12}
