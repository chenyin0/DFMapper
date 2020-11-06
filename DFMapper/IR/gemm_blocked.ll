; ModuleID = 'gemm.c'
source_filename = "gemm.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @bbgemm(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2) local_unnamed_addr #0 {
  br label %4

4:                                                ; preds = %3, %40
  %5 = phi i64 [ 0, %3 ], [ %41, %40 ]
  br label %6

6:                                                ; preds = %4, %37
  %7 = phi i64 [ 0, %4 ], [ %38, %37 ]
  br label %8

8:                                                ; preds = %6, %34
  %9 = phi i64 [ 0, %6 ], [ %35, %34 ]
  %10 = shl nsw i64 %9, 6
  br label %11

11:                                               ; preds = %8, %31
  %12 = phi i64 [ 0, %8 ], [ %32, %31 ]
  %13 = add nuw nsw i64 %12, %7
  %14 = shl nsw i64 %13, 6
  %15 = add nuw nsw i64 %13, %10
  %16 = getelementptr inbounds double, double* %0, i64 %15
  %17 = load double, double* %16, align 8, !tbaa !2
  br label %18

18:                                               ; preds = %11, %18
  %19 = phi i64 [ 0, %11 ], [ %29, %18 ]
  %20 = add nuw nsw i64 %19, %5
  %21 = add nuw nsw i64 %20, %14
  %22 = getelementptr inbounds double, double* %1, i64 %21
  %23 = load double, double* %22, align 8, !tbaa !2
  %24 = fmul double %17, %23
  %25 = add nuw nsw i64 %20, %10
  %26 = getelementptr inbounds double, double* %2, i64 %25
  %27 = load double, double* %26, align 8, !tbaa !2
  %28 = fadd double %27, %24
  store double %28, double* %26, align 8, !tbaa !2
  %29 = add nuw nsw i64 %19, 1
  %30 = icmp eq i64 %29, 8
  br i1 %30, label %31, label %18

31:                                               ; preds = %18
  %32 = add nuw nsw i64 %12, 1
  %33 = icmp eq i64 %32, 8
  br i1 %33, label %34, label %11

34:                                               ; preds = %31
  %35 = add nuw nsw i64 %9, 1
  %36 = icmp eq i64 %35, 64
  br i1 %36, label %37, label %8

37:                                               ; preds = %34
  %38 = add nuw nsw i64 %7, 8
  %39 = icmp ult i64 %7, 56
  br i1 %39, label %6, label %40

40:                                               ; preds = %37
  %41 = add nuw nsw i64 %5, 8
  %42 = icmp ult i64 %5, 56
  br i1 %42, label %4, label %43

43:                                               ; preds = %40
  ret void
}

attributes #0 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"double", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
