; ModuleID = 'spmv.c'
source_filename = "spmv.c"
target datalayout = "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-unknown-linux-gnu"

; Function Attrs: nofree noinline norecurse nounwind
define dso_local void @spmv(double* nocapture readonly %val, i32* nocapture readonly %cols, i32* nocapture readonly %rowDelimiters, double* nocapture readonly %vec, double* nocapture %out) local_unnamed_addr #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.end
  %i.031 = phi i32 [ 0, %entry ], [ %add, %for.end ]
  %arrayidx = getelementptr inbounds i32, i32* %rowDelimiters, i32 %i.031
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !3
  %add = add nuw nsw i32 %i.031, 1
  %arrayidx1 = getelementptr inbounds i32, i32* %rowDelimiters, i32 %add
  %1 = load i32, i32* %arrayidx1, align 4, !tbaa !3
  %cmp328 = icmp slt i32 %0, %1
  br i1 %cmp328, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.body, %for.body4
  %j.030 = phi i32 [ %inc, %for.body4 ], [ %0, %for.body ]
  %sum.029 = phi double [ %add8, %for.body4 ], [ 0.000000e+00, %for.body ]
  %arrayidx5 = getelementptr inbounds double, double* %val, i32 %j.030
  %2 = load double, double* %arrayidx5, align 4, !tbaa !7
  %arrayidx6 = getelementptr inbounds i32, i32* %cols, i32 %j.030
  %3 = load i32, i32* %arrayidx6, align 4, !tbaa !3
  %arrayidx7 = getelementptr inbounds double, double* %vec, i32 %3
  %4 = load double, double* %arrayidx7, align 4, !tbaa !7
  %mul = fmul double %2, %4
  %add8 = fadd double %sum.029, %mul
  %inc = add nsw i32 %j.030, 1
  %exitcond.not = icmp eq i32 %inc, %1
  br i1 %exitcond.not, label %for.end, label %for.body4, !llvm.loop !9

for.end:                                          ; preds = %for.body4, %for.body
  %sum.0.lcssa = phi double [ 0.000000e+00, %for.body ], [ %add8, %for.body4 ]
  %arrayidx9 = getelementptr inbounds double, double* %out, i32 %i.031
  store double %sum.0.lcssa, double* %arrayidx9, align 4, !tbaa !7
  %exitcond32.not = icmp eq i32 %add, 494
  br i1 %exitcond32.not, label %for.end12, label %for.body, !llvm.loop !12

for.end12:                                        ; preds = %for.end
  ret void
}

; Function Attrs: nofree noinline norecurse nounwind
define dso_local void @top() local_unnamed_addr #0 {
entry:
  call void @spmv(double* nonnull inttoptr (i32 268566720 to double*), i32* nonnull inttoptr (i32 268580096 to i32*), i32* nonnull inttoptr (i32 268586816 to i32*), double* nonnull inttoptr (i32 268588800 to double*), double* nonnull inttoptr (i32 268592768 to double*)) #1
  ret void
}

attributes #0 = { nofree noinline norecurse nounwind "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-builtins" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin "no-builtins" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 12.0.1 (https://github.com/llvm/llvm-project.git fed41342a82f5a3a9201819a82bf7a48313e296b)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!8, !8, i64 0}
!8 = !{!"double", !5, i64 0}
!9 = distinct !{!9, !10, !11}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.unroll.disable"}
!12 = distinct !{!12, !10, !11}
