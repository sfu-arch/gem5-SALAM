; ModuleID = 'conv0.c'
source_filename = "conv0.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

; Function Attrs: nofree norecurse nounwind
define dso_local void @compute([32 x [1 x i32]]* nocapture readonly %convInput, [5 x [1 x [6 x i32]]]* nocapture readonly %kernel, [28 x [6 x i32]]* nocapture %convOut) local_unnamed_addr #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.inc39
  %h.075 = phi i32 [ 0, %entry ], [ %inc40, %for.inc39 ]
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.cond1.preheader, %for.inc36
  %w.074 = phi i32 [ 0, %for.cond1.preheader ], [ %inc37, %for.inc36 ]
  %add.4 = add nuw nsw i32 %h.075, 4
  %arrayidx18.488 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.4, i32 %w.074, i32 0
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.cond4.preheader, %for.cond7.preheader
  %cc.073 = phi i32 [ 0, %for.cond4.preheader ], [ %inc34, %for.cond7.preheader ]
  %add.1 = add nuw nsw i32 %h.075, 1
  %add.2 = add nuw nsw i32 %h.075, 2
  %add.3 = add nuw nsw i32 %h.075, 3
  %arrayidx22.4.3 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 3, i32 4, i32 0, i32 %cc.073
  %0 = load i32, i32* %arrayidx22.4.3, align 4, !tbaa !3
  %add16.4.3 = add nuw nsw i32 %w.074, 4
  %arrayidx18.4.3 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.3, i32 %add16.4.3, i32 0
  %1 = load i32, i32* %arrayidx18.4.3, align 4, !tbaa !3
  %mul.4.3 = mul i32 %0, %1
  %arrayidx22.3.3 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 3, i32 3, i32 0, i32 %cc.073
  %2 = load i32, i32* %arrayidx22.3.3, align 4, !tbaa !3
  %add16.3.3 = add nuw nsw i32 %w.074, 3
  %arrayidx18.3.3 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.3, i32 %add16.3.3, i32 0
  %3 = load i32, i32* %arrayidx18.3.3, align 4, !tbaa !3
  %mul.3.3 = mul i32 %2, %3
  %arrayidx22.2.3 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 3, i32 2, i32 0, i32 %cc.073
  %4 = load i32, i32* %arrayidx22.2.3, align 4, !tbaa !3
  %add16.2.3 = add nuw nsw i32 %w.074, 2
  %arrayidx18.2.3 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.3, i32 %add16.2.3, i32 0
  %5 = load i32, i32* %arrayidx18.2.3, align 4, !tbaa !3
  %mul.2.3 = mul i32 %4, %5
  %arrayidx22.1.3 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 3, i32 1, i32 0, i32 %cc.073
  %6 = load i32, i32* %arrayidx22.1.3, align 4, !tbaa !3
  %add16.1.3 = add nuw nsw i32 %w.074, 1
  %arrayidx18.1.3 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.3, i32 %add16.1.3, i32 0
  %7 = load i32, i32* %arrayidx18.1.3, align 4, !tbaa !3
  %mul.1.3 = mul i32 %6, %7
  %arrayidx22.385 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 3, i32 0, i32 0, i32 %cc.073
  %8 = load i32, i32* %arrayidx22.385, align 4, !tbaa !3
  %arrayidx18.384 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.3, i32 %w.074, i32 0
  %9 = load i32, i32* %arrayidx18.384, align 4, !tbaa !3
  %mul.386 = mul i32 %8, %9
  %arrayidx22.4.2 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 2, i32 4, i32 0, i32 %cc.073
  %10 = load i32, i32* %arrayidx22.4.2, align 4, !tbaa !3
  %add16.4.2 = add nuw nsw i32 %w.074, 4
  %arrayidx18.4.2 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.2, i32 %add16.4.2, i32 0
  %11 = load i32, i32* %arrayidx18.4.2, align 4, !tbaa !3
  %mul.4.2 = mul i32 %10, %11
  %arrayidx22.3.2 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 2, i32 3, i32 0, i32 %cc.073
  %12 = load i32, i32* %arrayidx22.3.2, align 4, !tbaa !3
  %add16.3.2 = add nuw nsw i32 %w.074, 3
  %arrayidx18.3.2 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.2, i32 %add16.3.2, i32 0
  %13 = load i32, i32* %arrayidx18.3.2, align 4, !tbaa !3
  %mul.3.2 = mul i32 %12, %13
  %arrayidx22.2.2 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 2, i32 2, i32 0, i32 %cc.073
  %14 = load i32, i32* %arrayidx22.2.2, align 4, !tbaa !3
  %add16.2.2 = add nuw nsw i32 %w.074, 2
  %arrayidx18.2.2 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.2, i32 %add16.2.2, i32 0
  %15 = load i32, i32* %arrayidx18.2.2, align 4, !tbaa !3
  %mul.2.2 = mul i32 %14, %15
  %arrayidx22.1.2 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 2, i32 1, i32 0, i32 %cc.073
  %16 = load i32, i32* %arrayidx22.1.2, align 4, !tbaa !3
  %add16.1.2 = add nuw nsw i32 %w.074, 1
  %arrayidx18.1.2 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.2, i32 %add16.1.2, i32 0
  %17 = load i32, i32* %arrayidx18.1.2, align 4, !tbaa !3
  %mul.1.2 = mul i32 %16, %17
  %arrayidx22.281 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 2, i32 0, i32 0, i32 %cc.073
  %18 = load i32, i32* %arrayidx22.281, align 4, !tbaa !3
  %arrayidx18.280 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.2, i32 %w.074, i32 0
  %19 = load i32, i32* %arrayidx18.280, align 4, !tbaa !3
  %mul.282 = mul i32 %18, %19
  %arrayidx22.4.1 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 1, i32 4, i32 0, i32 %cc.073
  %20 = load i32, i32* %arrayidx22.4.1, align 4, !tbaa !3
  %add16.4.1 = add nuw nsw i32 %w.074, 4
  %arrayidx18.4.1 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.1, i32 %add16.4.1, i32 0
  %21 = load i32, i32* %arrayidx18.4.1, align 4, !tbaa !3
  %mul.4.1 = mul i32 %20, %21
  %arrayidx22.3.1 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 1, i32 3, i32 0, i32 %cc.073
  %22 = load i32, i32* %arrayidx22.3.1, align 4, !tbaa !3
  %add16.3.1 = add nuw nsw i32 %w.074, 3
  %arrayidx18.3.1 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.1, i32 %add16.3.1, i32 0
  %23 = load i32, i32* %arrayidx18.3.1, align 4, !tbaa !3
  %mul.3.1 = mul i32 %22, %23
  %arrayidx22.2.1 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 1, i32 2, i32 0, i32 %cc.073
  %24 = load i32, i32* %arrayidx22.2.1, align 4, !tbaa !3
  %add16.2.1 = add nuw nsw i32 %w.074, 2
  %arrayidx18.2.1 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.1, i32 %add16.2.1, i32 0
  %25 = load i32, i32* %arrayidx18.2.1, align 4, !tbaa !3
  %mul.2.1 = mul i32 %24, %25
  %arrayidx22.1.1 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 1, i32 1, i32 0, i32 %cc.073
  %26 = load i32, i32* %arrayidx22.1.1, align 4, !tbaa !3
  %add16.1.1 = add nuw nsw i32 %w.074, 1
  %arrayidx18.1.1 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.1, i32 %add16.1.1, i32 0
  %27 = load i32, i32* %arrayidx18.1.1, align 4, !tbaa !3
  %mul.1.1 = mul i32 %26, %27
  %arrayidx22.177 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 1, i32 0, i32 0, i32 %cc.073
  %28 = load i32, i32* %arrayidx22.177, align 4, !tbaa !3
  %arrayidx18.176 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.1, i32 %w.074, i32 0
  %29 = load i32, i32* %arrayidx18.176, align 4, !tbaa !3
  %mul.178 = mul i32 %28, %29
  %arrayidx22.4 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 0, i32 4, i32 0, i32 %cc.073
  %30 = load i32, i32* %arrayidx22.4, align 4, !tbaa !3
  %add16.4 = add nuw nsw i32 %w.074, 4
  %arrayidx18.4 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %h.075, i32 %add16.4, i32 0
  %31 = load i32, i32* %arrayidx18.4, align 4, !tbaa !3
  %mul.4 = mul i32 %30, %31
  %arrayidx22.3 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 0, i32 3, i32 0, i32 %cc.073
  %32 = load i32, i32* %arrayidx22.3, align 4, !tbaa !3
  %add16.3 = add nuw nsw i32 %w.074, 3
  %arrayidx18.3 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %h.075, i32 %add16.3, i32 0
  %33 = load i32, i32* %arrayidx18.3, align 4, !tbaa !3
  %mul.3 = mul i32 %32, %33
  %arrayidx22.2 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 0, i32 2, i32 0, i32 %cc.073
  %34 = load i32, i32* %arrayidx22.2, align 4, !tbaa !3
  %add16.2 = add nuw nsw i32 %w.074, 2
  %arrayidx18.2 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %h.075, i32 %add16.2, i32 0
  %35 = load i32, i32* %arrayidx18.2, align 4, !tbaa !3
  %mul.2 = mul i32 %34, %35
  %arrayidx22.1 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 0, i32 1, i32 0, i32 %cc.073
  %36 = load i32, i32* %arrayidx22.1, align 4, !tbaa !3
  %add16.1 = add nuw nsw i32 %w.074, 1
  %arrayidx18.1 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %h.075, i32 %add16.1, i32 0
  %37 = load i32, i32* %arrayidx18.1, align 4, !tbaa !3
  %mul.1 = mul i32 %36, %37
  %arrayidx22 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 0, i32 0, i32 0, i32 %cc.073
  %38 = load i32, i32* %arrayidx22, align 4, !tbaa !3
  %arrayidx18 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %h.075, i32 %w.074, i32 0
  %39 = load i32, i32* %arrayidx18, align 4, !tbaa !3
  %mul = mul i32 %38, %39
  %add23.1 = add i32 %mul.1, %mul
  %add23.2 = add i32 %mul.2, %add23.1
  %add23.3 = add i32 %mul.3, %add23.2
  %add23.4 = add i32 %mul.4, %add23.3
  %add23.179 = add i32 %mul.178, %add23.4
  %add23.1.1 = add i32 %mul.1.1, %add23.179
  %add23.2.1 = add i32 %mul.2.1, %add23.1.1
  %add23.3.1 = add i32 %mul.3.1, %add23.2.1
  %add23.4.1 = add i32 %mul.4.1, %add23.3.1
  %add23.283 = add i32 %mul.282, %add23.4.1
  %add23.1.2 = add i32 %mul.1.2, %add23.283
  %add23.2.2 = add i32 %mul.2.2, %add23.1.2
  %add23.3.2 = add i32 %mul.3.2, %add23.2.2
  %add23.4.2 = add i32 %mul.4.2, %add23.3.2
  %add23.387 = add i32 %mul.386, %add23.4.2
  %add23.1.3 = add i32 %mul.1.3, %add23.387
  %add23.2.3 = add i32 %mul.2.3, %add23.1.3
  %add23.3.3 = add i32 %mul.3.3, %add23.2.3
  %add23.4.3 = add i32 %mul.4.3, %add23.3.3
  %40 = load i32, i32* %arrayidx18.488, align 4, !tbaa !3
  %arrayidx22.489 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 4, i32 0, i32 0, i32 %cc.073
  %41 = load i32, i32* %arrayidx22.489, align 4, !tbaa !3
  %mul.490 = mul i32 %41, %40
  %add23.491 = add i32 %mul.490, %add23.4.3
  %add16.1.4 = add nuw nsw i32 %w.074, 1
  %arrayidx18.1.4 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.4, i32 %add16.1.4, i32 0
  %42 = load i32, i32* %arrayidx18.1.4, align 4, !tbaa !3
  %arrayidx22.1.4 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 4, i32 1, i32 0, i32 %cc.073
  %43 = load i32, i32* %arrayidx22.1.4, align 4, !tbaa !3
  %mul.1.4 = mul i32 %43, %42
  %add23.1.4 = add i32 %mul.1.4, %add23.491
  %add16.2.4 = add nuw nsw i32 %w.074, 2
  %arrayidx18.2.4 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.4, i32 %add16.2.4, i32 0
  %44 = load i32, i32* %arrayidx18.2.4, align 4, !tbaa !3
  %arrayidx22.2.4 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 4, i32 2, i32 0, i32 %cc.073
  %45 = load i32, i32* %arrayidx22.2.4, align 4, !tbaa !3
  %mul.2.4 = mul i32 %45, %44
  %add23.2.4 = add i32 %mul.2.4, %add23.1.4
  %add16.3.4 = add nuw nsw i32 %w.074, 3
  %arrayidx18.3.4 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.4, i32 %add16.3.4, i32 0
  %46 = load i32, i32* %arrayidx18.3.4, align 4, !tbaa !3
  %arrayidx22.3.4 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 4, i32 3, i32 0, i32 %cc.073
  %47 = load i32, i32* %arrayidx22.3.4, align 4, !tbaa !3
  %mul.3.4 = mul i32 %47, %46
  %add23.3.4 = add i32 %mul.3.4, %add23.2.4
  %add16.4.4 = add nuw nsw i32 %w.074, 4
  %arrayidx18.4.4 = getelementptr inbounds [32 x [1 x i32]], [32 x [1 x i32]]* %convInput, i32 %add.4, i32 %add16.4.4, i32 0
  %48 = load i32, i32* %arrayidx18.4.4, align 4, !tbaa !3
  %arrayidx22.4.4 = getelementptr inbounds [5 x [1 x [6 x i32]]], [5 x [1 x [6 x i32]]]* %kernel, i32 4, i32 4, i32 0, i32 %cc.073
  %49 = load i32, i32* %arrayidx22.4.4, align 4, !tbaa !3
  %mul.4.4 = mul i32 %49, %48
  %add23.4.4 = add i32 %mul.4.4, %add23.3.4
  %arrayidx32 = getelementptr inbounds [28 x [6 x i32]], [28 x [6 x i32]]* %convOut, i32 %h.075, i32 %w.074, i32 %cc.073
  store i32 %add23.4.4, i32* %arrayidx32, align 4, !tbaa !3
  %inc34 = add nuw nsw i32 %cc.073, 1
  %exitcond.not = icmp eq i32 %inc34, 6
  br i1 %exitcond.not, label %for.inc36, label %for.cond7.preheader, !llvm.loop !7

for.inc36:                                        ; preds = %for.cond7.preheader
  %inc37 = add nuw nsw i32 %w.074, 1
  %exitcond92.not = icmp eq i32 %inc37, 28
  br i1 %exitcond92.not, label %for.inc39, label %for.cond4.preheader, !llvm.loop !10

for.inc39:                                        ; preds = %for.inc36
  %inc40 = add nuw nsw i32 %h.075, 1
  %exitcond93.not = icmp eq i32 %inc40, 28
  br i1 %exitcond93.not, label %for.end41, label %for.cond1.preheader, !llvm.loop !11

for.end41:                                        ; preds = %for.inc39
  ret void
}

; Function Attrs: nofree norecurse nounwind
define dso_local void @top() local_unnamed_addr #0 {
entry:
  call void @compute([32 x [1 x i32]]* nonnull inttoptr (i32 268566720 to [32 x [1 x i32]]*), [5 x [1 x [6 x i32]]]* nonnull inttoptr (i32 268570880 to [5 x [1 x [6 x i32]]]*), [28 x [6 x i32]]* nonnull inttoptr (i32 268571520 to [28 x [6 x i32]]*))
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
