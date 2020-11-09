; ModuleID = 'backprop.c'
source_filename = "backprop.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: alwaysinline nofree nounwind optsize uwtable
define dso_local void @soft_max(double* nocapture %0, double* nocapture readonly %1) local_unnamed_addr #0 {
  br label %3

3:                                                ; preds = %2, %3
  %4 = phi i64 [ 0, %2 ], [ %11, %3 ]
  %5 = phi double [ 0.000000e+00, %2 ], [ %10, %3 ]
  %6 = getelementptr inbounds double, double* %1, i64 %4
  %7 = load double, double* %6, align 8, !tbaa !2
  %8 = fneg double %7
  %9 = tail call double @exp(double %8) #6
  %10 = fadd double %5, %9
  %11 = add nuw nsw i64 %4, 1
  %12 = icmp eq i64 %11, 3
  br i1 %12, label %13, label %3

13:                                               ; preds = %3, %13
  %14 = phi i64 [ %21, %13 ], [ 0, %3 ]
  %15 = getelementptr inbounds double, double* %1, i64 %14
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = fneg double %16
  %18 = tail call double @exp(double %17) #6
  %19 = fdiv double %18, %10
  %20 = getelementptr inbounds double, double* %0, i64 %14
  store double %19, double* %20, align 8, !tbaa !2
  %21 = add nuw nsw i64 %14, 1
  %22 = icmp eq i64 %21, 3
  br i1 %22, label %23, label %13

23:                                               ; preds = %13
  ret void
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree nounwind optsize
declare dso_local double @exp(double) local_unnamed_addr #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: alwaysinline nofree nounwind optsize uwtable
define dso_local void @RELU(double* nocapture %0, double* nocapture %1, i32 %2) local_unnamed_addr #0 {
  %4 = icmp sgt i32 %2, 0
  br i1 %4, label %5, label %21

5:                                                ; preds = %3
  %6 = zext i32 %2 to i64
  br label %7

7:                                                ; preds = %5, %7
  %8 = phi i64 [ 0, %5 ], [ %19, %7 ]
  %9 = getelementptr inbounds double, double* %0, i64 %8
  %10 = load double, double* %9, align 8, !tbaa !2
  %11 = fsub double 1.000000e+00, %10
  %12 = fmul double %10, %11
  %13 = getelementptr inbounds double, double* %1, i64 %8
  store double %12, double* %13, align 8, !tbaa !2
  %14 = load double, double* %9, align 8, !tbaa !2
  %15 = fneg double %14
  %16 = tail call double @exp(double %15) #6
  %17 = fadd double %16, 1.000000e+00
  %18 = fdiv double 1.000000e+00, %17
  store double %18, double* %9, align 8, !tbaa !2
  %19 = add nuw nsw i64 %8, 1
  %20 = icmp eq i64 %19, %6
  br i1 %20, label %21, label %7

21:                                               ; preds = %7, %3
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @add_bias_to_activations(double* nocapture readonly %0, double* nocapture %1, i32 %2) local_unnamed_addr #3 {
  %4 = icmp sgt i32 %2, 0
  br i1 %4, label %5, label %16

5:                                                ; preds = %3
  %6 = zext i32 %2 to i64
  br label %7

7:                                                ; preds = %5, %7
  %8 = phi i64 [ 0, %5 ], [ %14, %7 ]
  %9 = getelementptr inbounds double, double* %1, i64 %8
  %10 = load double, double* %9, align 8, !tbaa !2
  %11 = getelementptr inbounds double, double* %0, i64 %8
  %12 = load double, double* %11, align 8, !tbaa !2
  %13 = fadd double %10, %12
  store double %13, double* %9, align 8, !tbaa !2
  %14 = add nuw nsw i64 %8, 1
  %15 = icmp eq i64 %14, %6
  br i1 %15, label %16, label %7

16:                                               ; preds = %7, %3
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @matrix_vector_product_with_bias_input_layer(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2, double* nocapture readonly %3) local_unnamed_addr #3 {
  br label %5

5:                                                ; preds = %4, %21
  %6 = phi i64 [ 0, %4 ], [ %22, %21 ]
  %7 = getelementptr inbounds double, double* %2, i64 %6
  store double 0.000000e+00, double* %7, align 8, !tbaa !2
  %8 = mul nuw nsw i64 %6, 13
  br label %9

9:                                                ; preds = %5, %9
  %10 = phi double [ 0.000000e+00, %5 ], [ %18, %9 ]
  %11 = phi i64 [ 0, %5 ], [ %19, %9 ]
  %12 = add nuw nsw i64 %11, %8
  %13 = getelementptr inbounds double, double* %1, i64 %12
  %14 = load double, double* %13, align 8, !tbaa !2
  %15 = getelementptr inbounds double, double* %3, i64 %11
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = fmul double %14, %16
  %18 = fadd double %10, %17
  store double %18, double* %7, align 8, !tbaa !2
  %19 = add nuw nsw i64 %11, 1
  %20 = icmp eq i64 %19, 13
  br i1 %20, label %21, label %9

21:                                               ; preds = %9
  %22 = add nuw nsw i64 %6, 1
  %23 = icmp eq i64 %22, 64
  br i1 %23, label %24, label %5

24:                                               ; preds = %21, %24
  %25 = phi i64 [ %31, %24 ], [ 0, %21 ]
  %26 = getelementptr inbounds double, double* %2, i64 %25
  %27 = load double, double* %26, align 8, !tbaa !2
  %28 = getelementptr inbounds double, double* %0, i64 %25
  %29 = load double, double* %28, align 8, !tbaa !2
  %30 = fadd double %27, %29
  store double %30, double* %26, align 8, !tbaa !2
  %31 = add nuw nsw i64 %25, 1
  %32 = icmp eq i64 %31, 64
  br i1 %32, label %33, label %24

33:                                               ; preds = %24
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @matrix_vector_product_with_bias_second_layer(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2, double* nocapture readonly %3) local_unnamed_addr #3 {
  br label %5

5:                                                ; preds = %4, %21
  %6 = phi i64 [ 0, %4 ], [ %22, %21 ]
  %7 = getelementptr inbounds double, double* %2, i64 %6
  store double 0.000000e+00, double* %7, align 8, !tbaa !2
  %8 = shl nsw i64 %6, 6
  br label %9

9:                                                ; preds = %5, %9
  %10 = phi double [ 0.000000e+00, %5 ], [ %18, %9 ]
  %11 = phi i64 [ 0, %5 ], [ %19, %9 ]
  %12 = add nuw nsw i64 %11, %8
  %13 = getelementptr inbounds double, double* %1, i64 %12
  %14 = load double, double* %13, align 8, !tbaa !2
  %15 = getelementptr inbounds double, double* %3, i64 %11
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = fmul double %14, %16
  %18 = fadd double %10, %17
  store double %18, double* %7, align 8, !tbaa !2
  %19 = add nuw nsw i64 %11, 1
  %20 = icmp eq i64 %19, 64
  br i1 %20, label %21, label %9

21:                                               ; preds = %9
  %22 = add nuw nsw i64 %6, 1
  %23 = icmp eq i64 %22, 64
  br i1 %23, label %24, label %5

24:                                               ; preds = %21, %24
  %25 = phi i64 [ %31, %24 ], [ 0, %21 ]
  %26 = getelementptr inbounds double, double* %2, i64 %25
  %27 = load double, double* %26, align 8, !tbaa !2
  %28 = getelementptr inbounds double, double* %0, i64 %25
  %29 = load double, double* %28, align 8, !tbaa !2
  %30 = fadd double %27, %29
  store double %30, double* %26, align 8, !tbaa !2
  %31 = add nuw nsw i64 %25, 1
  %32 = icmp eq i64 %31, 64
  br i1 %32, label %33, label %24

33:                                               ; preds = %24
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @matrix_vector_product_with_bias_output_layer(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2, double* nocapture readonly %3) local_unnamed_addr #3 {
  br label %5

5:                                                ; preds = %4, %21
  %6 = phi i64 [ 0, %4 ], [ %22, %21 ]
  %7 = getelementptr inbounds double, double* %2, i64 %6
  store double 0.000000e+00, double* %7, align 8, !tbaa !2
  %8 = shl nsw i64 %6, 6
  br label %9

9:                                                ; preds = %5, %9
  %10 = phi double [ 0.000000e+00, %5 ], [ %18, %9 ]
  %11 = phi i64 [ 0, %5 ], [ %19, %9 ]
  %12 = add nuw nsw i64 %11, %8
  %13 = getelementptr inbounds double, double* %1, i64 %12
  %14 = load double, double* %13, align 8, !tbaa !2
  %15 = getelementptr inbounds double, double* %3, i64 %11
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = fmul double %14, %16
  %18 = fadd double %10, %17
  store double %18, double* %7, align 8, !tbaa !2
  %19 = add nuw nsw i64 %11, 1
  %20 = icmp eq i64 %19, 64
  br i1 %20, label %21, label %9

21:                                               ; preds = %9
  %22 = add nuw nsw i64 %6, 1
  %23 = icmp eq i64 %22, 3
  br i1 %23, label %24, label %5

24:                                               ; preds = %21, %24
  %25 = phi i64 [ %31, %24 ], [ 0, %21 ]
  %26 = getelementptr inbounds double, double* %2, i64 %25
  %27 = load double, double* %26, align 8, !tbaa !2
  %28 = getelementptr inbounds double, double* %0, i64 %25
  %29 = load double, double* %28, align 8, !tbaa !2
  %30 = fadd double %27, %29
  store double %30, double* %26, align 8, !tbaa !2
  %31 = add nuw nsw i64 %25, 1
  %32 = icmp eq i64 %31, 3
  br i1 %32, label %33, label %24

33:                                               ; preds = %24
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @take_difference(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2, double* nocapture readonly %3) local_unnamed_addr #3 {
  br label %5

5:                                                ; preds = %4, %5
  %6 = phi i64 [ 0, %4 ], [ %17, %5 ]
  %7 = getelementptr inbounds double, double* %0, i64 %6
  %8 = load double, double* %7, align 8, !tbaa !2
  %9 = getelementptr inbounds double, double* %1, i64 %6
  %10 = load double, double* %9, align 8, !tbaa !2
  %11 = fsub double %8, %10
  %12 = fneg double %11
  %13 = getelementptr inbounds double, double* %3, i64 %6
  %14 = load double, double* %13, align 8, !tbaa !2
  %15 = fmul double %14, %12
  %16 = getelementptr inbounds double, double* %2, i64 %6
  store double %15, double* %16, align 8, !tbaa !2
  %17 = add nuw nsw i64 %6, 1
  %18 = icmp eq i64 %17, 3
  br i1 %18, label %19, label %5

19:                                               ; preds = %5
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @get_delta_matrix_weights3(double* nocapture %0, double* nocapture readonly %1, double* nocapture readonly %2) local_unnamed_addr #3 {
  br label %4

4:                                                ; preds = %3, %18
  %5 = phi i64 [ 0, %3 ], [ %19, %18 ]
  %6 = getelementptr inbounds double, double* %2, i64 %5
  %7 = mul nuw nsw i64 %5, 3
  br label %8

8:                                                ; preds = %4, %8
  %9 = phi i64 [ 0, %4 ], [ %16, %8 ]
  %10 = load double, double* %6, align 8, !tbaa !2
  %11 = getelementptr inbounds double, double* %1, i64 %9
  %12 = load double, double* %11, align 8, !tbaa !2
  %13 = fmul double %10, %12
  %14 = add nuw nsw i64 %9, %7
  %15 = getelementptr inbounds double, double* %0, i64 %14
  store double %13, double* %15, align 8, !tbaa !2
  %16 = add nuw nsw i64 %9, 1
  %17 = icmp eq i64 %16, 3
  br i1 %17, label %18, label %8

18:                                               ; preds = %8
  %19 = add nuw nsw i64 %5, 1
  %20 = icmp eq i64 %19, 64
  br i1 %20, label %21, label %4

21:                                               ; preds = %18
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @get_oracle_activations2(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2, double* nocapture readonly %3) local_unnamed_addr #3 {
  br label %5

5:                                                ; preds = %4, %21
  %6 = phi i64 [ 0, %4 ], [ %25, %21 ]
  %7 = getelementptr inbounds double, double* %2, i64 %6
  store double 0.000000e+00, double* %7, align 8, !tbaa !2
  %8 = mul nuw nsw i64 %6, 3
  br label %9

9:                                                ; preds = %5, %9
  %10 = phi double [ 0.000000e+00, %5 ], [ %18, %9 ]
  %11 = phi i64 [ 0, %5 ], [ %19, %9 ]
  %12 = getelementptr inbounds double, double* %1, i64 %11
  %13 = load double, double* %12, align 8, !tbaa !2
  %14 = add nuw nsw i64 %11, %8
  %15 = getelementptr inbounds double, double* %0, i64 %14
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = fmul double %13, %16
  %18 = fadd double %10, %17
  store double %18, double* %7, align 8, !tbaa !2
  %19 = add nuw nsw i64 %11, 1
  %20 = icmp eq i64 %19, 3
  br i1 %20, label %21, label %9

21:                                               ; preds = %9
  %22 = getelementptr inbounds double, double* %3, i64 %6
  %23 = load double, double* %22, align 8, !tbaa !2
  %24 = fmul double %18, %23
  store double %24, double* %7, align 8, !tbaa !2
  %25 = add nuw nsw i64 %6, 1
  %26 = icmp eq i64 %25, 64
  br i1 %26, label %27, label %5

27:                                               ; preds = %21
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @get_delta_matrix_weights2(double* nocapture %0, double* nocapture readonly %1, double* nocapture readonly %2) local_unnamed_addr #3 {
  br label %4

4:                                                ; preds = %3, %18
  %5 = phi i64 [ 0, %3 ], [ %19, %18 ]
  %6 = getelementptr inbounds double, double* %2, i64 %5
  %7 = shl nsw i64 %5, 6
  br label %8

8:                                                ; preds = %4, %8
  %9 = phi i64 [ 0, %4 ], [ %16, %8 ]
  %10 = load double, double* %6, align 8, !tbaa !2
  %11 = getelementptr inbounds double, double* %1, i64 %9
  %12 = load double, double* %11, align 8, !tbaa !2
  %13 = fmul double %10, %12
  %14 = add nuw nsw i64 %9, %7
  %15 = getelementptr inbounds double, double* %0, i64 %14
  store double %13, double* %15, align 8, !tbaa !2
  %16 = add nuw nsw i64 %9, 1
  %17 = icmp eq i64 %16, 64
  br i1 %17, label %18, label %8

18:                                               ; preds = %8
  %19 = add nuw nsw i64 %5, 1
  %20 = icmp eq i64 %19, 64
  br i1 %20, label %21, label %4

21:                                               ; preds = %18
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @get_oracle_activations1(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2, double* nocapture readonly %3) local_unnamed_addr #3 {
  br label %5

5:                                                ; preds = %4, %21
  %6 = phi i64 [ 0, %4 ], [ %25, %21 ]
  %7 = getelementptr inbounds double, double* %2, i64 %6
  store double 0.000000e+00, double* %7, align 8, !tbaa !2
  %8 = shl nsw i64 %6, 6
  br label %9

9:                                                ; preds = %5, %9
  %10 = phi double [ 0.000000e+00, %5 ], [ %18, %9 ]
  %11 = phi i64 [ 0, %5 ], [ %19, %9 ]
  %12 = getelementptr inbounds double, double* %1, i64 %11
  %13 = load double, double* %12, align 8, !tbaa !2
  %14 = add nuw nsw i64 %11, %8
  %15 = getelementptr inbounds double, double* %0, i64 %14
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = fmul double %13, %16
  %18 = fadd double %10, %17
  store double %18, double* %7, align 8, !tbaa !2
  %19 = add nuw nsw i64 %11, 1
  %20 = icmp eq i64 %19, 64
  br i1 %20, label %21, label %9

21:                                               ; preds = %9
  %22 = getelementptr inbounds double, double* %3, i64 %6
  %23 = load double, double* %22, align 8, !tbaa !2
  %24 = fmul double %18, %23
  store double %24, double* %7, align 8, !tbaa !2
  %25 = add nuw nsw i64 %6, 1
  %26 = icmp eq i64 %25, 64
  br i1 %26, label %27, label %5

27:                                               ; preds = %21
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @get_delta_matrix_weights1(double* nocapture %0, double* nocapture readonly %1, double* nocapture readonly %2) local_unnamed_addr #3 {
  br label %4

4:                                                ; preds = %3, %18
  %5 = phi i64 [ 0, %3 ], [ %19, %18 ]
  %6 = getelementptr inbounds double, double* %2, i64 %5
  %7 = shl nsw i64 %5, 6
  br label %8

8:                                                ; preds = %4, %8
  %9 = phi i64 [ 0, %4 ], [ %16, %8 ]
  %10 = load double, double* %6, align 8, !tbaa !2
  %11 = getelementptr inbounds double, double* %1, i64 %9
  %12 = load double, double* %11, align 8, !tbaa !2
  %13 = fmul double %10, %12
  %14 = add nuw nsw i64 %9, %7
  %15 = getelementptr inbounds double, double* %0, i64 %14
  store double %13, double* %15, align 8, !tbaa !2
  %16 = add nuw nsw i64 %9, 1
  %17 = icmp eq i64 %16, 64
  br i1 %17, label %18, label %8

18:                                               ; preds = %8
  %19 = add nuw nsw i64 %5, 1
  %20 = icmp eq i64 %19, 13
  br i1 %20, label %21, label %4

21:                                               ; preds = %18
  ret void
}

; Function Attrs: alwaysinline nofree nounwind optsize uwtable
define dso_local void @update_weights(double* nocapture %0, double* nocapture %1, double* nocapture %2, double* nocapture readonly %3, double* nocapture readonly %4, double* nocapture readonly %5, double* nocapture %6, double* nocapture %7, double* nocapture %8, double* nocapture readonly %9, double* nocapture readonly %10, double* nocapture readonly %11) local_unnamed_addr #0 {
  br label %13

13:                                               ; preds = %12, %31
  %14 = phi i64 [ 0, %12 ], [ %32, %31 ]
  %15 = phi double [ 0.000000e+00, %12 ], [ %28, %31 ]
  %16 = shl nsw i64 %14, 6
  br label %17

17:                                               ; preds = %13, %17
  %18 = phi i64 [ 0, %13 ], [ %29, %17 ]
  %19 = phi double [ %15, %13 ], [ %28, %17 ]
  %20 = add nuw nsw i64 %18, %16
  %21 = getelementptr inbounds double, double* %3, i64 %20
  %22 = load double, double* %21, align 8, !tbaa !2
  %23 = fmul double %22, 1.000000e-02
  %24 = getelementptr inbounds double, double* %0, i64 %20
  %25 = load double, double* %24, align 8, !tbaa !2
  %26 = fsub double %25, %23
  store double %26, double* %24, align 8, !tbaa !2
  %27 = fmul double %26, %26
  %28 = fadd double %19, %27
  %29 = add nuw nsw i64 %18, 1
  %30 = icmp eq i64 %29, 64
  br i1 %30, label %31, label %17

31:                                               ; preds = %17
  %32 = add nuw nsw i64 %14, 1
  %33 = icmp eq i64 %32, 13
  br i1 %33, label %34, label %13

34:                                               ; preds = %31, %34
  %35 = phi i64 [ %45, %34 ], [ 0, %31 ]
  %36 = phi double [ %44, %34 ], [ 0.000000e+00, %31 ]
  %37 = getelementptr inbounds double, double* %9, i64 %35
  %38 = load double, double* %37, align 8, !tbaa !2
  %39 = fmul double %38, 1.000000e-02
  %40 = getelementptr inbounds double, double* %6, i64 %35
  %41 = load double, double* %40, align 8, !tbaa !2
  %42 = fsub double %41, %39
  store double %42, double* %40, align 8, !tbaa !2
  %43 = fmul double %42, %42
  %44 = fadd double %36, %43
  %45 = add nuw nsw i64 %35, 1
  %46 = icmp eq i64 %45, 64
  br i1 %46, label %47, label %34

47:                                               ; preds = %34
  %48 = tail call double @sqrt(double %28) #6
  %49 = tail call double @sqrt(double %44) #6
  %50 = insertelement <2 x double> undef, double %48, i32 0
  %51 = shufflevector <2 x double> %50, <2 x double> undef, <2 x i32> zeroinitializer
  br label %52

52:                                               ; preds = %47, %65
  %53 = phi i64 [ 0, %47 ], [ %66, %65 ]
  %54 = shl nsw i64 %53, 6
  br label %55

55:                                               ; preds = %55, %52
  %56 = phi i64 [ 0, %52 ], [ %63, %55 ]
  %57 = add nuw nsw i64 %56, %54
  %58 = getelementptr inbounds double, double* %0, i64 %57
  %59 = bitcast double* %58 to <2 x double>*
  %60 = load <2 x double>, <2 x double>* %59, align 8, !tbaa !2
  %61 = fdiv <2 x double> %60, %51
  %62 = bitcast double* %58 to <2 x double>*
  store <2 x double> %61, <2 x double>* %62, align 8, !tbaa !2
  %63 = add i64 %56, 2
  %64 = icmp eq i64 %63, 64
  br i1 %64, label %65, label %55, !llvm.loop !6

65:                                               ; preds = %55
  %66 = add nuw nsw i64 %53, 1
  %67 = icmp eq i64 %66, 13
  br i1 %67, label %68, label %52

68:                                               ; preds = %65
  %69 = insertelement <2 x double> undef, double %49, i32 0
  %70 = shufflevector <2 x double> %69, <2 x double> undef, <2 x i32> zeroinitializer
  br label %71

71:                                               ; preds = %71, %68
  %72 = phi i64 [ 0, %68 ], [ %78, %71 ]
  %73 = getelementptr inbounds double, double* %6, i64 %72
  %74 = bitcast double* %73 to <2 x double>*
  %75 = load <2 x double>, <2 x double>* %74, align 8, !tbaa !2
  %76 = fdiv <2 x double> %75, %70
  %77 = bitcast double* %73 to <2 x double>*
  store <2 x double> %76, <2 x double>* %77, align 8, !tbaa !2
  %78 = add i64 %72, 2
  %79 = icmp eq i64 %78, 64
  br i1 %79, label %80, label %71, !llvm.loop !8

80:                                               ; preds = %71, %98
  %81 = phi i64 [ %99, %98 ], [ 0, %71 ]
  %82 = phi double [ %95, %98 ], [ 0.000000e+00, %71 ]
  %83 = shl nsw i64 %81, 6
  br label %84

84:                                               ; preds = %80, %84
  %85 = phi i64 [ 0, %80 ], [ %96, %84 ]
  %86 = phi double [ %82, %80 ], [ %95, %84 ]
  %87 = add nuw nsw i64 %85, %83
  %88 = getelementptr inbounds double, double* %4, i64 %87
  %89 = load double, double* %88, align 8, !tbaa !2
  %90 = fmul double %89, 1.000000e-02
  %91 = getelementptr inbounds double, double* %1, i64 %87
  %92 = load double, double* %91, align 8, !tbaa !2
  %93 = fsub double %92, %90
  store double %93, double* %91, align 8, !tbaa !2
  %94 = fmul double %93, %93
  %95 = fadd double %86, %94
  %96 = add nuw nsw i64 %85, 1
  %97 = icmp eq i64 %96, 64
  br i1 %97, label %98, label %84

98:                                               ; preds = %84
  %99 = add nuw nsw i64 %81, 1
  %100 = icmp eq i64 %99, 64
  br i1 %100, label %101, label %80

101:                                              ; preds = %98, %101
  %102 = phi i64 [ %112, %101 ], [ 0, %98 ]
  %103 = phi double [ %111, %101 ], [ 0.000000e+00, %98 ]
  %104 = getelementptr inbounds double, double* %10, i64 %102
  %105 = load double, double* %104, align 8, !tbaa !2
  %106 = fmul double %105, 1.000000e-02
  %107 = getelementptr inbounds double, double* %7, i64 %102
  %108 = load double, double* %107, align 8, !tbaa !2
  %109 = fsub double %108, %106
  store double %109, double* %107, align 8, !tbaa !2
  %110 = fmul double %109, %109
  %111 = fadd double %103, %110
  %112 = add nuw nsw i64 %102, 1
  %113 = icmp eq i64 %112, 64
  br i1 %113, label %114, label %101

114:                                              ; preds = %101
  %115 = tail call double @sqrt(double %95) #6
  %116 = tail call double @sqrt(double %111) #6
  %117 = insertelement <2 x double> undef, double %115, i32 0
  %118 = shufflevector <2 x double> %117, <2 x double> undef, <2 x i32> zeroinitializer
  br label %119

119:                                              ; preds = %114, %132
  %120 = phi i64 [ 0, %114 ], [ %133, %132 ]
  %121 = shl nsw i64 %120, 6
  br label %122

122:                                              ; preds = %122, %119
  %123 = phi i64 [ 0, %119 ], [ %130, %122 ]
  %124 = add nuw nsw i64 %123, %121
  %125 = getelementptr inbounds double, double* %1, i64 %124
  %126 = bitcast double* %125 to <2 x double>*
  %127 = load <2 x double>, <2 x double>* %126, align 8, !tbaa !2
  %128 = fdiv <2 x double> %127, %118
  %129 = bitcast double* %125 to <2 x double>*
  store <2 x double> %128, <2 x double>* %129, align 8, !tbaa !2
  %130 = add i64 %123, 2
  %131 = icmp eq i64 %130, 64
  br i1 %131, label %132, label %122, !llvm.loop !9

132:                                              ; preds = %122
  %133 = add nuw nsw i64 %120, 1
  %134 = icmp eq i64 %133, 64
  br i1 %134, label %135, label %119

135:                                              ; preds = %132
  %136 = insertelement <2 x double> undef, double %116, i32 0
  %137 = shufflevector <2 x double> %136, <2 x double> undef, <2 x i32> zeroinitializer
  br label %138

138:                                              ; preds = %138, %135
  %139 = phi i64 [ 0, %135 ], [ %145, %138 ]
  %140 = getelementptr inbounds double, double* %7, i64 %139
  %141 = bitcast double* %140 to <2 x double>*
  %142 = load <2 x double>, <2 x double>* %141, align 8, !tbaa !2
  %143 = fdiv <2 x double> %142, %137
  %144 = bitcast double* %140 to <2 x double>*
  store <2 x double> %143, <2 x double>* %144, align 8, !tbaa !2
  %145 = add i64 %139, 2
  %146 = icmp eq i64 %145, 64
  br i1 %146, label %147, label %138, !llvm.loop !10

147:                                              ; preds = %138, %165
  %148 = phi i64 [ %166, %165 ], [ 0, %138 ]
  %149 = phi double [ %162, %165 ], [ 0.000000e+00, %138 ]
  %150 = mul nuw nsw i64 %148, 3
  br label %151

151:                                              ; preds = %147, %151
  %152 = phi i64 [ 0, %147 ], [ %163, %151 ]
  %153 = phi double [ %149, %147 ], [ %162, %151 ]
  %154 = add nuw nsw i64 %152, %150
  %155 = getelementptr inbounds double, double* %5, i64 %154
  %156 = load double, double* %155, align 8, !tbaa !2
  %157 = fmul double %156, 1.000000e-02
  %158 = getelementptr inbounds double, double* %2, i64 %154
  %159 = load double, double* %158, align 8, !tbaa !2
  %160 = fsub double %159, %157
  store double %160, double* %158, align 8, !tbaa !2
  %161 = fmul double %160, %160
  %162 = fadd double %153, %161
  %163 = add nuw nsw i64 %152, 1
  %164 = icmp eq i64 %163, 3
  br i1 %164, label %165, label %151

165:                                              ; preds = %151
  %166 = add nuw nsw i64 %148, 1
  %167 = icmp eq i64 %166, 64
  br i1 %167, label %168, label %147

168:                                              ; preds = %165, %168
  %169 = phi i64 [ %179, %168 ], [ 0, %165 ]
  %170 = phi double [ %178, %168 ], [ 0.000000e+00, %165 ]
  %171 = getelementptr inbounds double, double* %11, i64 %169
  %172 = load double, double* %171, align 8, !tbaa !2
  %173 = fmul double %172, 1.000000e-02
  %174 = getelementptr inbounds double, double* %8, i64 %169
  %175 = load double, double* %174, align 8, !tbaa !2
  %176 = fsub double %175, %173
  store double %176, double* %174, align 8, !tbaa !2
  %177 = fmul double %176, %176
  %178 = fadd double %170, %177
  %179 = add nuw nsw i64 %169, 1
  %180 = icmp eq i64 %179, 3
  br i1 %180, label %181, label %168

181:                                              ; preds = %168
  %182 = tail call double @sqrt(double %162) #6
  %183 = tail call double @sqrt(double %178) #6
  br label %184

184:                                              ; preds = %181, %195
  %185 = phi i64 [ 0, %181 ], [ %196, %195 ]
  %186 = mul nuw nsw i64 %185, 3
  br label %187

187:                                              ; preds = %184, %187
  %188 = phi i64 [ 0, %184 ], [ %193, %187 ]
  %189 = add nuw nsw i64 %188, %186
  %190 = getelementptr inbounds double, double* %2, i64 %189
  %191 = load double, double* %190, align 8, !tbaa !2
  %192 = fdiv double %191, %182
  store double %192, double* %190, align 8, !tbaa !2
  %193 = add nuw nsw i64 %188, 1
  %194 = icmp eq i64 %193, 3
  br i1 %194, label %195, label %187

195:                                              ; preds = %187
  %196 = add nuw nsw i64 %185, 1
  %197 = icmp eq i64 %196, 64
  br i1 %197, label %198, label %184

198:                                              ; preds = %195, %198
  %199 = phi i64 [ %203, %198 ], [ 0, %195 ]
  %200 = getelementptr inbounds double, double* %8, i64 %199
  %201 = load double, double* %200, align 8, !tbaa !2
  %202 = fdiv double %201, %183
  store double %202, double* %200, align 8, !tbaa !2
  %203 = add nuw nsw i64 %199, 1
  %204 = icmp eq i64 %203, 3
  br i1 %204, label %205, label %198

205:                                              ; preds = %198
  ret void
}

; Function Attrs: nofree nounwind optsize
declare dso_local double @sqrt(double) local_unnamed_addr #2

; Function Attrs: nounwind optsize uwtable
define dso_local void @backprop(double* nocapture %0, double* nocapture %1, double* nocapture %2, double* nocapture %3, double* nocapture %4, double* nocapture %5, double* nocapture readonly %6, double* nocapture readonly %7) local_unnamed_addr #4 {
  %9 = alloca [64 x double], align 16
  %10 = bitcast [64 x double]* %9 to i8*
  %11 = alloca [64 x double], align 16
  %12 = bitcast [64 x double]* %11 to i8*
  %13 = alloca [3 x double], align 16
  %14 = alloca [64 x double], align 16
  %15 = alloca [64 x double], align 16
  %16 = alloca [3 x double], align 16
  %17 = alloca [3 x double], align 16
  %18 = alloca [3 x double], align 16
  %19 = alloca [832 x double], align 16
  %20 = alloca [4096 x double], align 16
  %21 = alloca [192 x double], align 16
  %22 = alloca [64 x double], align 16
  %23 = alloca [64 x double], align 16
  call void @llvm.lifetime.start.p0i8(i64 512, i8* nonnull %10) #7
  call void @llvm.lifetime.start.p0i8(i64 512, i8* nonnull %12) #7
  %24 = bitcast [3 x double]* %13 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %24) #7
  %25 = bitcast [64 x double]* %14 to i8*
  call void @llvm.lifetime.start.p0i8(i64 512, i8* nonnull %25) #7
  %26 = bitcast [64 x double]* %15 to i8*
  call void @llvm.lifetime.start.p0i8(i64 512, i8* nonnull %26) #7
  %27 = bitcast [3 x double]* %16 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %27) #7
  %28 = bitcast [3 x double]* %17 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %28) #7
  %29 = bitcast [3 x double]* %18 to i8*
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %29) #7
  %30 = bitcast [832 x double]* %19 to i8*
  call void @llvm.lifetime.start.p0i8(i64 6656, i8* nonnull %30) #7
  %31 = bitcast [4096 x double]* %20 to i8*
  call void @llvm.lifetime.start.p0i8(i64 32768, i8* nonnull %31) #7
  %32 = bitcast [192 x double]* %21 to i8*
  call void @llvm.lifetime.start.p0i8(i64 1536, i8* nonnull %32) #7
  %33 = bitcast [64 x double]* %22 to i8*
  call void @llvm.lifetime.start.p0i8(i64 512, i8* nonnull %33) #7
  %34 = bitcast [64 x double]* %23 to i8*
  call void @llvm.lifetime.start.p0i8(i64 512, i8* nonnull %34) #7
  br label %35

35:                                               ; preds = %8, %509
  %36 = phi i64 [ 0, %8 ], [ %510, %509 ]
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 dereferenceable(512) %10, i8 0, i64 512, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 dereferenceable(512) %12, i8 0, i64 512, i1 false)
  br label %37

37:                                               ; preds = %35, %42
  %38 = phi i64 [ 0, %35 ], [ %43, %42 ]
  %39 = icmp ult i64 %38, 3
  br i1 %39, label %40, label %42

40:                                               ; preds = %37
  %41 = getelementptr inbounds [3 x double], [3 x double]* %13, i64 0, i64 %38
  store double 0.000000e+00, double* %41, align 8, !tbaa !2
  br label %42

42:                                               ; preds = %37, %40
  %43 = add nuw nsw i64 %38, 1
  %44 = icmp eq i64 %43, 64
  br i1 %44, label %45, label %37

45:                                               ; preds = %42
  %46 = mul nuw nsw i64 %36, 13
  %47 = getelementptr inbounds double, double* %6, i64 %46
  br label %48

48:                                               ; preds = %64, %45
  %49 = phi i64 [ 0, %45 ], [ %65, %64 ]
  %50 = getelementptr inbounds [64 x double], [64 x double]* %9, i64 0, i64 %49
  store double 0.000000e+00, double* %50, align 8, !tbaa !2
  %51 = mul nuw nsw i64 %49, 13
  br label %52

52:                                               ; preds = %52, %48
  %53 = phi double [ 0.000000e+00, %48 ], [ %61, %52 ]
  %54 = phi i64 [ 0, %48 ], [ %62, %52 ]
  %55 = add nuw nsw i64 %54, %51
  %56 = getelementptr inbounds double, double* %0, i64 %55
  %57 = load double, double* %56, align 8, !tbaa !2
  %58 = getelementptr inbounds double, double* %47, i64 %54
  %59 = load double, double* %58, align 8, !tbaa !2
  %60 = fmul double %57, %59
  %61 = fadd double %53, %60
  %62 = add nuw nsw i64 %54, 1
  %63 = icmp eq i64 %62, 13
  br i1 %63, label %64, label %52

64:                                               ; preds = %52
  store double %61, double* %50, align 8, !tbaa !2
  %65 = add nuw nsw i64 %49, 1
  %66 = icmp eq i64 %65, 64
  br i1 %66, label %67, label %48

67:                                               ; preds = %64, %67
  %68 = phi i64 [ %77, %67 ], [ 0, %64 ]
  %69 = getelementptr inbounds [64 x double], [64 x double]* %9, i64 0, i64 %68
  %70 = bitcast double* %69 to <2 x double>*
  %71 = load <2 x double>, <2 x double>* %70, align 16, !tbaa !2
  %72 = getelementptr inbounds double, double* %3, i64 %68
  %73 = bitcast double* %72 to <2 x double>*
  %74 = load <2 x double>, <2 x double>* %73, align 8, !tbaa !2
  %75 = fadd <2 x double> %71, %74
  %76 = bitcast double* %69 to <2 x double>*
  store <2 x double> %75, <2 x double>* %76, align 16, !tbaa !2
  %77 = add i64 %68, 2
  %78 = icmp eq i64 %77, 64
  br i1 %78, label %79, label %67, !llvm.loop !11

79:                                               ; preds = %67, %79
  %80 = phi i64 [ %90, %79 ], [ 0, %67 ]
  %81 = getelementptr inbounds [64 x double], [64 x double]* %9, i64 0, i64 %80
  %82 = load double, double* %81, align 8, !tbaa !2
  %83 = fsub double 1.000000e+00, %82
  %84 = fmul double %82, %83
  %85 = getelementptr inbounds [64 x double], [64 x double]* %14, i64 0, i64 %80
  store double %84, double* %85, align 8, !tbaa !2
  %86 = fneg double %82
  %87 = tail call double @exp(double %86) #6
  %88 = fadd double %87, 1.000000e+00
  %89 = fdiv double 1.000000e+00, %88
  store double %89, double* %81, align 8, !tbaa !2
  %90 = add nuw nsw i64 %80, 1
  %91 = icmp eq i64 %90, 64
  br i1 %91, label %92, label %79

92:                                               ; preds = %79, %108
  %93 = phi i64 [ %109, %108 ], [ 0, %79 ]
  %94 = getelementptr inbounds [64 x double], [64 x double]* %11, i64 0, i64 %93
  store double 0.000000e+00, double* %94, align 8, !tbaa !2
  %95 = shl nsw i64 %93, 6
  br label %96

96:                                               ; preds = %96, %92
  %97 = phi double [ 0.000000e+00, %92 ], [ %105, %96 ]
  %98 = phi i64 [ 0, %92 ], [ %106, %96 ]
  %99 = add nuw nsw i64 %98, %95
  %100 = getelementptr inbounds double, double* %1, i64 %99
  %101 = load double, double* %100, align 8, !tbaa !2
  %102 = getelementptr inbounds [64 x double], [64 x double]* %9, i64 0, i64 %98
  %103 = load double, double* %102, align 8, !tbaa !2
  %104 = fmul double %101, %103
  %105 = fadd double %97, %104
  %106 = add nuw nsw i64 %98, 1
  %107 = icmp eq i64 %106, 64
  br i1 %107, label %108, label %96

108:                                              ; preds = %96
  store double %105, double* %94, align 8, !tbaa !2
  %109 = add nuw nsw i64 %93, 1
  %110 = icmp eq i64 %109, 64
  br i1 %110, label %111, label %92

111:                                              ; preds = %108, %111
  %112 = phi i64 [ %121, %111 ], [ 0, %108 ]
  %113 = getelementptr inbounds [64 x double], [64 x double]* %11, i64 0, i64 %112
  %114 = bitcast double* %113 to <2 x double>*
  %115 = load <2 x double>, <2 x double>* %114, align 16, !tbaa !2
  %116 = getelementptr inbounds double, double* %4, i64 %112
  %117 = bitcast double* %116 to <2 x double>*
  %118 = load <2 x double>, <2 x double>* %117, align 8, !tbaa !2
  %119 = fadd <2 x double> %115, %118
  %120 = bitcast double* %113 to <2 x double>*
  store <2 x double> %119, <2 x double>* %120, align 16, !tbaa !2
  %121 = add i64 %112, 2
  %122 = icmp eq i64 %121, 64
  br i1 %122, label %123, label %111, !llvm.loop !12

123:                                              ; preds = %111, %123
  %124 = phi i64 [ %134, %123 ], [ 0, %111 ]
  %125 = getelementptr inbounds [64 x double], [64 x double]* %11, i64 0, i64 %124
  %126 = load double, double* %125, align 8, !tbaa !2
  %127 = fsub double 1.000000e+00, %126
  %128 = fmul double %126, %127
  %129 = getelementptr inbounds [64 x double], [64 x double]* %15, i64 0, i64 %124
  store double %128, double* %129, align 8, !tbaa !2
  %130 = fneg double %126
  %131 = tail call double @exp(double %130) #6
  %132 = fadd double %131, 1.000000e+00
  %133 = fdiv double 1.000000e+00, %132
  store double %133, double* %125, align 8, !tbaa !2
  %134 = add nuw nsw i64 %124, 1
  %135 = icmp eq i64 %134, 64
  br i1 %135, label %136, label %123

136:                                              ; preds = %123, %152
  %137 = phi i64 [ %153, %152 ], [ 0, %123 ]
  %138 = getelementptr inbounds [3 x double], [3 x double]* %13, i64 0, i64 %137
  store double 0.000000e+00, double* %138, align 8, !tbaa !2
  %139 = shl nsw i64 %137, 6
  br label %140

140:                                              ; preds = %140, %136
  %141 = phi double [ 0.000000e+00, %136 ], [ %149, %140 ]
  %142 = phi i64 [ 0, %136 ], [ %150, %140 ]
  %143 = add nuw nsw i64 %142, %139
  %144 = getelementptr inbounds double, double* %2, i64 %143
  %145 = load double, double* %144, align 8, !tbaa !2
  %146 = getelementptr inbounds [64 x double], [64 x double]* %11, i64 0, i64 %142
  %147 = load double, double* %146, align 8, !tbaa !2
  %148 = fmul double %145, %147
  %149 = fadd double %141, %148
  %150 = add nuw nsw i64 %142, 1
  %151 = icmp eq i64 %150, 64
  br i1 %151, label %152, label %140

152:                                              ; preds = %140
  store double %149, double* %138, align 8, !tbaa !2
  %153 = add nuw nsw i64 %137, 1
  %154 = icmp eq i64 %153, 3
  br i1 %154, label %155, label %136

155:                                              ; preds = %152, %155
  %156 = phi i64 [ %162, %155 ], [ 0, %152 ]
  %157 = getelementptr inbounds [3 x double], [3 x double]* %13, i64 0, i64 %156
  %158 = load double, double* %157, align 8, !tbaa !2
  %159 = getelementptr inbounds double, double* %5, i64 %156
  %160 = load double, double* %159, align 8, !tbaa !2
  %161 = fadd double %158, %160
  store double %161, double* %157, align 8, !tbaa !2
  %162 = add nuw nsw i64 %156, 1
  %163 = icmp eq i64 %162, 3
  br i1 %163, label %164, label %155

164:                                              ; preds = %155, %164
  %165 = phi i64 [ %175, %164 ], [ 0, %155 ]
  %166 = getelementptr inbounds [3 x double], [3 x double]* %13, i64 0, i64 %165
  %167 = load double, double* %166, align 8, !tbaa !2
  %168 = fsub double 1.000000e+00, %167
  %169 = fmul double %167, %168
  %170 = getelementptr inbounds [3 x double], [3 x double]* %16, i64 0, i64 %165
  store double %169, double* %170, align 8, !tbaa !2
  %171 = fneg double %167
  %172 = tail call double @exp(double %171) #6
  %173 = fadd double %172, 1.000000e+00
  %174 = fdiv double 1.000000e+00, %173
  store double %174, double* %166, align 8, !tbaa !2
  %175 = add nuw nsw i64 %165, 1
  %176 = icmp eq i64 %175, 3
  br i1 %176, label %177, label %164

177:                                              ; preds = %164, %177
  %178 = phi i64 [ %185, %177 ], [ 0, %164 ]
  %179 = phi double [ %184, %177 ], [ 0.000000e+00, %164 ]
  %180 = getelementptr inbounds [3 x double], [3 x double]* %13, i64 0, i64 %178
  %181 = load double, double* %180, align 8, !tbaa !2
  %182 = fneg double %181
  %183 = tail call double @exp(double %182) #6
  %184 = fadd double %179, %183
  %185 = add nuw nsw i64 %178, 1
  %186 = icmp eq i64 %185, 3
  br i1 %186, label %187, label %177

187:                                              ; preds = %177, %187
  %188 = phi i64 [ %195, %187 ], [ 0, %177 ]
  %189 = getelementptr inbounds [3 x double], [3 x double]* %13, i64 0, i64 %188
  %190 = load double, double* %189, align 8, !tbaa !2
  %191 = fneg double %190
  %192 = tail call double @exp(double %191) #6
  %193 = fdiv double %192, %184
  %194 = getelementptr inbounds [3 x double], [3 x double]* %17, i64 0, i64 %188
  store double %193, double* %194, align 8, !tbaa !2
  %195 = add nuw nsw i64 %188, 1
  %196 = icmp eq i64 %195, 3
  br i1 %196, label %197, label %187

197:                                              ; preds = %187
  %198 = mul nuw nsw i64 %36, 3
  %199 = getelementptr inbounds double, double* %7, i64 %198
  br label %200

200:                                              ; preds = %200, %197
  %201 = phi i64 [ 0, %197 ], [ %212, %200 ]
  %202 = getelementptr inbounds [3 x double], [3 x double]* %17, i64 0, i64 %201
  %203 = load double, double* %202, align 8, !tbaa !2
  %204 = getelementptr inbounds double, double* %199, i64 %201
  %205 = load double, double* %204, align 8, !tbaa !2
  %206 = fsub double %203, %205
  %207 = fneg double %206
  %208 = getelementptr inbounds [3 x double], [3 x double]* %16, i64 0, i64 %201
  %209 = load double, double* %208, align 8, !tbaa !2
  %210 = fmul double %209, %207
  %211 = getelementptr inbounds [3 x double], [3 x double]* %18, i64 0, i64 %201
  store double %210, double* %211, align 8, !tbaa !2
  %212 = add nuw nsw i64 %201, 1
  %213 = icmp eq i64 %212, 3
  br i1 %213, label %214, label %200

214:                                              ; preds = %200, %228
  %215 = phi i64 [ %229, %228 ], [ 0, %200 ]
  %216 = getelementptr inbounds [64 x double], [64 x double]* %11, i64 0, i64 %215
  %217 = mul nuw nsw i64 %215, 3
  %218 = load double, double* %216, align 8, !tbaa !2
  br label %219

219:                                              ; preds = %219, %214
  %220 = phi i64 [ 0, %214 ], [ %226, %219 ]
  %221 = getelementptr inbounds [3 x double], [3 x double]* %18, i64 0, i64 %220
  %222 = load double, double* %221, align 8, !tbaa !2
  %223 = fmul double %218, %222
  %224 = add nuw nsw i64 %220, %217
  %225 = getelementptr inbounds [192 x double], [192 x double]* %21, i64 0, i64 %224
  store double %223, double* %225, align 8, !tbaa !2
  %226 = add nuw nsw i64 %220, 1
  %227 = icmp eq i64 %226, 3
  br i1 %227, label %228, label %219

228:                                              ; preds = %219
  %229 = add nuw nsw i64 %215, 1
  %230 = icmp eq i64 %229, 64
  br i1 %230, label %231, label %214

231:                                              ; preds = %228, %247
  %232 = phi i64 [ %251, %247 ], [ 0, %228 ]
  %233 = getelementptr inbounds [64 x double], [64 x double]* %23, i64 0, i64 %232
  store double 0.000000e+00, double* %233, align 8, !tbaa !2
  %234 = mul nuw nsw i64 %232, 3
  br label %235

235:                                              ; preds = %235, %231
  %236 = phi double [ 0.000000e+00, %231 ], [ %244, %235 ]
  %237 = phi i64 [ 0, %231 ], [ %245, %235 ]
  %238 = getelementptr inbounds [3 x double], [3 x double]* %18, i64 0, i64 %237
  %239 = load double, double* %238, align 8, !tbaa !2
  %240 = add nuw nsw i64 %237, %234
  %241 = getelementptr inbounds double, double* %2, i64 %240
  %242 = load double, double* %241, align 8, !tbaa !2
  %243 = fmul double %239, %242
  %244 = fadd double %236, %243
  %245 = add nuw nsw i64 %237, 1
  %246 = icmp eq i64 %245, 3
  br i1 %246, label %247, label %235

247:                                              ; preds = %235
  %248 = getelementptr inbounds [64 x double], [64 x double]* %15, i64 0, i64 %232
  %249 = load double, double* %248, align 8, !tbaa !2
  %250 = fmul double %244, %249
  store double %250, double* %233, align 8, !tbaa !2
  %251 = add nuw nsw i64 %232, 1
  %252 = icmp eq i64 %251, 64
  br i1 %252, label %253, label %231

253:                                              ; preds = %247, %271
  %254 = phi i64 [ %272, %271 ], [ 0, %247 ]
  %255 = shl nsw i64 %254, 6
  %256 = getelementptr inbounds [64 x double], [64 x double]* %9, i64 0, i64 %254
  %257 = load double, double* %256, align 8, !tbaa !2
  %258 = insertelement <2 x double> undef, double %257, i32 0
  %259 = shufflevector <2 x double> %258, <2 x double> undef, <2 x i32> zeroinitializer
  br label %260

260:                                              ; preds = %260, %253
  %261 = phi i64 [ 0, %253 ], [ %269, %260 ]
  %262 = getelementptr inbounds [64 x double], [64 x double]* %23, i64 0, i64 %261
  %263 = bitcast double* %262 to <2 x double>*
  %264 = load <2 x double>, <2 x double>* %263, align 16, !tbaa !2
  %265 = fmul <2 x double> %259, %264
  %266 = add nuw nsw i64 %261, %255
  %267 = getelementptr inbounds [4096 x double], [4096 x double]* %20, i64 0, i64 %266
  %268 = bitcast double* %267 to <2 x double>*
  store <2 x double> %265, <2 x double>* %268, align 16, !tbaa !2
  %269 = add i64 %261, 2
  %270 = icmp eq i64 %269, 64
  br i1 %270, label %271, label %260, !llvm.loop !13

271:                                              ; preds = %260
  %272 = add nuw nsw i64 %254, 1
  %273 = icmp eq i64 %272, 64
  br i1 %273, label %274, label %253

274:                                              ; preds = %271, %290
  %275 = phi i64 [ %294, %290 ], [ 0, %271 ]
  %276 = getelementptr inbounds [64 x double], [64 x double]* %22, i64 0, i64 %275
  store double 0.000000e+00, double* %276, align 8, !tbaa !2
  %277 = shl nsw i64 %275, 6
  br label %278

278:                                              ; preds = %278, %274
  %279 = phi double [ 0.000000e+00, %274 ], [ %287, %278 ]
  %280 = phi i64 [ 0, %274 ], [ %288, %278 ]
  %281 = getelementptr inbounds [64 x double], [64 x double]* %23, i64 0, i64 %280
  %282 = load double, double* %281, align 8, !tbaa !2
  %283 = add nuw nsw i64 %280, %277
  %284 = getelementptr inbounds double, double* %1, i64 %283
  %285 = load double, double* %284, align 8, !tbaa !2
  %286 = fmul double %282, %285
  %287 = fadd double %279, %286
  %288 = add nuw nsw i64 %280, 1
  %289 = icmp eq i64 %288, 64
  br i1 %289, label %290, label %278

290:                                              ; preds = %278
  %291 = getelementptr inbounds [64 x double], [64 x double]* %14, i64 0, i64 %275
  %292 = load double, double* %291, align 8, !tbaa !2
  %293 = fmul double %287, %292
  store double %293, double* %276, align 8, !tbaa !2
  %294 = add nuw nsw i64 %275, 1
  %295 = icmp eq i64 %294, 64
  br i1 %295, label %296, label %274

296:                                              ; preds = %290, %314
  %297 = phi i64 [ %315, %314 ], [ 0, %290 ]
  %298 = shl nsw i64 %297, 6
  %299 = getelementptr inbounds double, double* %47, i64 %297
  %300 = load double, double* %299, align 8, !tbaa !2
  %301 = insertelement <2 x double> undef, double %300, i32 0
  %302 = shufflevector <2 x double> %301, <2 x double> undef, <2 x i32> zeroinitializer
  br label %303

303:                                              ; preds = %303, %296
  %304 = phi i64 [ 0, %296 ], [ %312, %303 ]
  %305 = getelementptr inbounds [64 x double], [64 x double]* %22, i64 0, i64 %304
  %306 = bitcast double* %305 to <2 x double>*
  %307 = load <2 x double>, <2 x double>* %306, align 16, !tbaa !2
  %308 = fmul <2 x double> %302, %307
  %309 = add nuw nsw i64 %304, %298
  %310 = getelementptr inbounds [832 x double], [832 x double]* %19, i64 0, i64 %309
  %311 = bitcast double* %310 to <2 x double>*
  store <2 x double> %308, <2 x double>* %311, align 16, !tbaa !2
  %312 = add i64 %304, 2
  %313 = icmp eq i64 %312, 64
  br i1 %313, label %314, label %303, !llvm.loop !14

314:                                              ; preds = %303
  %315 = add nuw nsw i64 %297, 1
  %316 = icmp eq i64 %315, 13
  br i1 %316, label %317, label %296

317:                                              ; preds = %314, %335
  %318 = phi i64 [ %336, %335 ], [ 0, %314 ]
  %319 = phi double [ %332, %335 ], [ 0.000000e+00, %314 ]
  %320 = shl nsw i64 %318, 6
  br label %321

321:                                              ; preds = %321, %317
  %322 = phi i64 [ 0, %317 ], [ %333, %321 ]
  %323 = phi double [ %319, %317 ], [ %332, %321 ]
  %324 = add nuw nsw i64 %322, %320
  %325 = getelementptr inbounds [832 x double], [832 x double]* %19, i64 0, i64 %324
  %326 = load double, double* %325, align 8, !tbaa !2
  %327 = fmul double %326, 1.000000e-02
  %328 = getelementptr inbounds double, double* %0, i64 %324
  %329 = load double, double* %328, align 8, !tbaa !2
  %330 = fsub double %329, %327
  store double %330, double* %328, align 8, !tbaa !2
  %331 = fmul double %330, %330
  %332 = fadd double %323, %331
  %333 = add nuw nsw i64 %322, 1
  %334 = icmp eq i64 %333, 64
  br i1 %334, label %335, label %321

335:                                              ; preds = %321
  %336 = add nuw nsw i64 %318, 1
  %337 = icmp eq i64 %336, 13
  br i1 %337, label %338, label %317

338:                                              ; preds = %335, %338
  %339 = phi i64 [ %349, %338 ], [ 0, %335 ]
  %340 = phi double [ %348, %338 ], [ 0.000000e+00, %335 ]
  %341 = getelementptr inbounds [64 x double], [64 x double]* %22, i64 0, i64 %339
  %342 = load double, double* %341, align 8, !tbaa !2
  %343 = fmul double %342, 1.000000e-02
  %344 = getelementptr inbounds double, double* %3, i64 %339
  %345 = load double, double* %344, align 8, !tbaa !2
  %346 = fsub double %345, %343
  store double %346, double* %344, align 8, !tbaa !2
  %347 = fmul double %346, %346
  %348 = fadd double %340, %347
  %349 = add nuw nsw i64 %339, 1
  %350 = icmp eq i64 %349, 64
  br i1 %350, label %351, label %338

351:                                              ; preds = %338
  %352 = tail call double @sqrt(double %332) #6
  %353 = tail call double @sqrt(double %348) #6
  %354 = insertelement <2 x double> undef, double %352, i32 0
  %355 = shufflevector <2 x double> %354, <2 x double> undef, <2 x i32> zeroinitializer
  br label %356

356:                                              ; preds = %369, %351
  %357 = phi i64 [ 0, %351 ], [ %370, %369 ]
  %358 = shl nsw i64 %357, 6
  br label %359

359:                                              ; preds = %359, %356
  %360 = phi i64 [ 0, %356 ], [ %367, %359 ]
  %361 = add nuw nsw i64 %360, %358
  %362 = getelementptr inbounds double, double* %0, i64 %361
  %363 = bitcast double* %362 to <2 x double>*
  %364 = load <2 x double>, <2 x double>* %363, align 8, !tbaa !2
  %365 = fdiv <2 x double> %364, %355
  %366 = bitcast double* %362 to <2 x double>*
  store <2 x double> %365, <2 x double>* %366, align 8, !tbaa !2
  %367 = add i64 %360, 2
  %368 = icmp eq i64 %367, 64
  br i1 %368, label %369, label %359, !llvm.loop !15

369:                                              ; preds = %359
  %370 = add nuw nsw i64 %357, 1
  %371 = icmp eq i64 %370, 13
  br i1 %371, label %372, label %356

372:                                              ; preds = %369
  %373 = insertelement <2 x double> undef, double %353, i32 0
  %374 = shufflevector <2 x double> %373, <2 x double> undef, <2 x i32> zeroinitializer
  br label %375

375:                                              ; preds = %375, %372
  %376 = phi i64 [ 0, %372 ], [ %382, %375 ]
  %377 = getelementptr inbounds double, double* %3, i64 %376
  %378 = bitcast double* %377 to <2 x double>*
  %379 = load <2 x double>, <2 x double>* %378, align 8, !tbaa !2
  %380 = fdiv <2 x double> %379, %374
  %381 = bitcast double* %377 to <2 x double>*
  store <2 x double> %380, <2 x double>* %381, align 8, !tbaa !2
  %382 = add i64 %376, 2
  %383 = icmp eq i64 %382, 64
  br i1 %383, label %384, label %375, !llvm.loop !16

384:                                              ; preds = %375, %402
  %385 = phi i64 [ %403, %402 ], [ 0, %375 ]
  %386 = phi double [ %399, %402 ], [ 0.000000e+00, %375 ]
  %387 = shl nsw i64 %385, 6
  br label %388

388:                                              ; preds = %388, %384
  %389 = phi i64 [ 0, %384 ], [ %400, %388 ]
  %390 = phi double [ %386, %384 ], [ %399, %388 ]
  %391 = add nuw nsw i64 %389, %387
  %392 = getelementptr inbounds [4096 x double], [4096 x double]* %20, i64 0, i64 %391
  %393 = load double, double* %392, align 8, !tbaa !2
  %394 = fmul double %393, 1.000000e-02
  %395 = getelementptr inbounds double, double* %1, i64 %391
  %396 = load double, double* %395, align 8, !tbaa !2
  %397 = fsub double %396, %394
  store double %397, double* %395, align 8, !tbaa !2
  %398 = fmul double %397, %397
  %399 = fadd double %390, %398
  %400 = add nuw nsw i64 %389, 1
  %401 = icmp eq i64 %400, 64
  br i1 %401, label %402, label %388

402:                                              ; preds = %388
  %403 = add nuw nsw i64 %385, 1
  %404 = icmp eq i64 %403, 64
  br i1 %404, label %405, label %384

405:                                              ; preds = %402, %405
  %406 = phi i64 [ %416, %405 ], [ 0, %402 ]
  %407 = phi double [ %415, %405 ], [ 0.000000e+00, %402 ]
  %408 = getelementptr inbounds [64 x double], [64 x double]* %23, i64 0, i64 %406
  %409 = load double, double* %408, align 8, !tbaa !2
  %410 = fmul double %409, 1.000000e-02
  %411 = getelementptr inbounds double, double* %4, i64 %406
  %412 = load double, double* %411, align 8, !tbaa !2
  %413 = fsub double %412, %410
  store double %413, double* %411, align 8, !tbaa !2
  %414 = fmul double %413, %413
  %415 = fadd double %407, %414
  %416 = add nuw nsw i64 %406, 1
  %417 = icmp eq i64 %416, 64
  br i1 %417, label %418, label %405

418:                                              ; preds = %405
  %419 = tail call double @sqrt(double %399) #6
  %420 = tail call double @sqrt(double %415) #6
  %421 = insertelement <2 x double> undef, double %419, i32 0
  %422 = shufflevector <2 x double> %421, <2 x double> undef, <2 x i32> zeroinitializer
  br label %423

423:                                              ; preds = %436, %418
  %424 = phi i64 [ 0, %418 ], [ %437, %436 ]
  %425 = shl nsw i64 %424, 6
  br label %426

426:                                              ; preds = %426, %423
  %427 = phi i64 [ 0, %423 ], [ %434, %426 ]
  %428 = add nuw nsw i64 %427, %425
  %429 = getelementptr inbounds double, double* %1, i64 %428
  %430 = bitcast double* %429 to <2 x double>*
  %431 = load <2 x double>, <2 x double>* %430, align 8, !tbaa !2
  %432 = fdiv <2 x double> %431, %422
  %433 = bitcast double* %429 to <2 x double>*
  store <2 x double> %432, <2 x double>* %433, align 8, !tbaa !2
  %434 = add i64 %427, 2
  %435 = icmp eq i64 %434, 64
  br i1 %435, label %436, label %426, !llvm.loop !17

436:                                              ; preds = %426
  %437 = add nuw nsw i64 %424, 1
  %438 = icmp eq i64 %437, 64
  br i1 %438, label %439, label %423

439:                                              ; preds = %436
  %440 = insertelement <2 x double> undef, double %420, i32 0
  %441 = shufflevector <2 x double> %440, <2 x double> undef, <2 x i32> zeroinitializer
  br label %442

442:                                              ; preds = %442, %439
  %443 = phi i64 [ 0, %439 ], [ %449, %442 ]
  %444 = getelementptr inbounds double, double* %4, i64 %443
  %445 = bitcast double* %444 to <2 x double>*
  %446 = load <2 x double>, <2 x double>* %445, align 8, !tbaa !2
  %447 = fdiv <2 x double> %446, %441
  %448 = bitcast double* %444 to <2 x double>*
  store <2 x double> %447, <2 x double>* %448, align 8, !tbaa !2
  %449 = add i64 %443, 2
  %450 = icmp eq i64 %449, 64
  br i1 %450, label %451, label %442, !llvm.loop !18

451:                                              ; preds = %442, %469
  %452 = phi i64 [ %470, %469 ], [ 0, %442 ]
  %453 = phi double [ %466, %469 ], [ 0.000000e+00, %442 ]
  %454 = mul nuw nsw i64 %452, 3
  br label %455

455:                                              ; preds = %455, %451
  %456 = phi i64 [ 0, %451 ], [ %467, %455 ]
  %457 = phi double [ %453, %451 ], [ %466, %455 ]
  %458 = add nuw nsw i64 %456, %454
  %459 = getelementptr inbounds [192 x double], [192 x double]* %21, i64 0, i64 %458
  %460 = load double, double* %459, align 8, !tbaa !2
  %461 = fmul double %460, 1.000000e-02
  %462 = getelementptr inbounds double, double* %2, i64 %458
  %463 = load double, double* %462, align 8, !tbaa !2
  %464 = fsub double %463, %461
  store double %464, double* %462, align 8, !tbaa !2
  %465 = fmul double %464, %464
  %466 = fadd double %457, %465
  %467 = add nuw nsw i64 %456, 1
  %468 = icmp eq i64 %467, 3
  br i1 %468, label %469, label %455

469:                                              ; preds = %455
  %470 = add nuw nsw i64 %452, 1
  %471 = icmp eq i64 %470, 64
  br i1 %471, label %472, label %451

472:                                              ; preds = %469, %472
  %473 = phi i64 [ %483, %472 ], [ 0, %469 ]
  %474 = phi double [ %482, %472 ], [ 0.000000e+00, %469 ]
  %475 = getelementptr inbounds [3 x double], [3 x double]* %18, i64 0, i64 %473
  %476 = load double, double* %475, align 8, !tbaa !2
  %477 = fmul double %476, 1.000000e-02
  %478 = getelementptr inbounds double, double* %5, i64 %473
  %479 = load double, double* %478, align 8, !tbaa !2
  %480 = fsub double %479, %477
  store double %480, double* %478, align 8, !tbaa !2
  %481 = fmul double %480, %480
  %482 = fadd double %474, %481
  %483 = add nuw nsw i64 %473, 1
  %484 = icmp eq i64 %483, 3
  br i1 %484, label %485, label %472

485:                                              ; preds = %472
  %486 = tail call double @sqrt(double %466) #6
  %487 = tail call double @sqrt(double %482) #6
  br label %488

488:                                              ; preds = %499, %485
  %489 = phi i64 [ 0, %485 ], [ %500, %499 ]
  %490 = mul nuw nsw i64 %489, 3
  br label %491

491:                                              ; preds = %491, %488
  %492 = phi i64 [ 0, %488 ], [ %497, %491 ]
  %493 = add nuw nsw i64 %492, %490
  %494 = getelementptr inbounds double, double* %2, i64 %493
  %495 = load double, double* %494, align 8, !tbaa !2
  %496 = fdiv double %495, %486
  store double %496, double* %494, align 8, !tbaa !2
  %497 = add nuw nsw i64 %492, 1
  %498 = icmp eq i64 %497, 3
  br i1 %498, label %499, label %491

499:                                              ; preds = %491
  %500 = add nuw nsw i64 %489, 1
  %501 = icmp eq i64 %500, 64
  br i1 %501, label %502, label %488

502:                                              ; preds = %499, %502
  %503 = phi i64 [ %507, %502 ], [ 0, %499 ]
  %504 = getelementptr inbounds double, double* %5, i64 %503
  %505 = load double, double* %504, align 8, !tbaa !2
  %506 = fdiv double %505, %487
  store double %506, double* %504, align 8, !tbaa !2
  %507 = add nuw nsw i64 %503, 1
  %508 = icmp eq i64 %507, 3
  br i1 %508, label %509, label %502

509:                                              ; preds = %502
  %510 = add nuw nsw i64 %36, 1
  %511 = icmp eq i64 %510, 163
  br i1 %511, label %512, label %35

512:                                              ; preds = %509
  call void @llvm.lifetime.end.p0i8(i64 512, i8* nonnull %34) #7
  call void @llvm.lifetime.end.p0i8(i64 512, i8* nonnull %33) #7
  call void @llvm.lifetime.end.p0i8(i64 1536, i8* nonnull %32) #7
  call void @llvm.lifetime.end.p0i8(i64 32768, i8* nonnull %31) #7
  call void @llvm.lifetime.end.p0i8(i64 6656, i8* nonnull %30) #7
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %29) #7
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %28) #7
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %27) #7
  call void @llvm.lifetime.end.p0i8(i64 512, i8* nonnull %26) #7
  call void @llvm.lifetime.end.p0i8(i64 512, i8* nonnull %25) #7
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %24) #7
  call void @llvm.lifetime.end.p0i8(i64 512, i8* nonnull %12) #7
  call void @llvm.lifetime.end.p0i8(i64 512, i8* nonnull %10) #7
  ret void
}

; Function Attrs: argmemonly nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #5

attributes #0 = { alwaysinline nofree nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nofree nounwind optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { alwaysinline nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { argmemonly nounwind willreturn writeonly }
attributes #6 = { nounwind optsize }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"double", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.isvectorized", i32 1}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
!16 = distinct !{!16, !7}
!17 = distinct !{!17, !7}
!18 = distinct !{!18, !7}
