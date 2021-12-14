; ModuleID = 'pool1.c'
source_filename = "pool1.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

; Function Attrs: nofree norecurse nounwind
define dso_local void @compute([10 x [16 x i32]]* nocapture readonly %poolIn, [5 x [16 x i32]]* nocapture %poolOut) local_unnamed_addr #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.inc31
  %h.060 = phi i32 [ 0, %entry ], [ %add32, %for.inc31 ]
  %div20 = lshr exact i32 %h.060, 1
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.cond1.preheader, %for.inc28
  %w.059 = phi i32 [ 0, %for.cond1.preheader ], [ %add29, %for.inc28 ]
  %div22 = lshr exact i32 %w.059, 1
  %add.1 = or i32 %h.060, 1
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.cond4.preheader, %for.cond7.preheader
  %c.058 = phi i32 [ 0, %for.cond4.preheader ], [ %inc26, %for.cond7.preheader ]
  %add13.1 = or i32 %w.059, 1
  %arrayidx15.1 = getelementptr inbounds [10 x [16 x i32]], [10 x [16 x i32]]* %poolIn, i32 %h.060, i32 %add13.1, i32 %c.058
  %0 = load i32, i32* %arrayidx15.1, align 4, !tbaa !3
  %arrayidx15 = getelementptr inbounds [10 x [16 x i32]], [10 x [16 x i32]]* %poolIn, i32 %h.060, i32 %w.059, i32 %c.058
  %1 = load i32, i32* %arrayidx15, align 4, !tbaa !3
  %add16.1 = add i32 %0, %1
  %arrayidx15.161 = getelementptr inbounds [10 x [16 x i32]], [10 x [16 x i32]]* %poolIn, i32 %add.1, i32 %w.059, i32 %c.058
  %2 = load i32, i32* %arrayidx15.161, align 4, !tbaa !3
  %add16.162 = add i32 %2, %add16.1
  %add13.1.1 = or i32 %w.059, 1
  %arrayidx15.1.1 = getelementptr inbounds [10 x [16 x i32]], [10 x [16 x i32]]* %poolIn, i32 %add.1, i32 %add13.1.1, i32 %c.058
  %3 = load i32, i32* %arrayidx15.1.1, align 4, !tbaa !3
  %add16.1.1 = add i32 %3, %add16.162
  %div = sdiv i32 %add16.1.1, 4
  %arrayidx24 = getelementptr inbounds [5 x [16 x i32]], [5 x [16 x i32]]* %poolOut, i32 %div20, i32 %div22, i32 %c.058
  store i32 %div, i32* %arrayidx24, align 4, !tbaa !3
  %inc26 = add nuw nsw i32 %c.058, 1
  %exitcond.not = icmp eq i32 %inc26, 16
  br i1 %exitcond.not, label %for.inc28, label %for.cond7.preheader, !llvm.loop !7

for.inc28:                                        ; preds = %for.cond7.preheader
  %add29 = add nuw nsw i32 %w.059, 2
  %cmp2 = icmp ult i32 %w.059, 8
  br i1 %cmp2, label %for.cond4.preheader, label %for.inc31, !llvm.loop !10

for.inc31:                                        ; preds = %for.inc28
  %add32 = add nuw nsw i32 %h.060, 2
  %cmp = icmp ult i32 %h.060, 8
  br i1 %cmp, label %for.cond1.preheader, label %for.end33, !llvm.loop !11

for.end33:                                        ; preds = %for.inc31
  ret void
}

; Function Attrs: nofree norecurse nounwind
define dso_local void @top() local_unnamed_addr #0 {
entry:
  call void @compute([10 x [16 x i32]]* nonnull inttoptr (i32 268635072 to [10 x [16 x i32]]*), [5 x [16 x i32]]* nonnull inttoptr (i32 268641536 to [5 x [16 x i32]]*))
  ret void
}

attributes #0 = { nofree norecurse nounwind "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+strict-align,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 12.0.1 (https://github.com/llvm/llvm-project.git fed41342a82f5a3a9201819a82bf7a48313e296b)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = distinct !{!7, !8, !9}
!8 = !{!"llvm.loop.mustprogress"}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = distinct !{!10, !8, !9}
!11 = distinct !{!11, !8, !9}
