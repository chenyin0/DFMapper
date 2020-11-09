; ModuleID = 'gemm.c'
source_filename = "gemm.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @gemm(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2) local_unnamed_addr #0 {
  br label %4

4:                                                ; preds = %3, %28
  %5 = phi i64 [ 0, %3 ], [ %29, %28 ]
  %6 = shl nsw i64 %5, 6
  br label %7

7:                                                ; preds = %4, %23
  %8 = phi i64 [ 0, %4 ], [ %26, %23 ]
  br label %9

9:                                                ; preds = %7, %9
  %10 = phi i64 [ 0, %7 ], [ %21, %9 ]
  %11 = phi double [ 0.000000e+00, %7 ], [ %20, %9 ]
  %12 = shl nuw nsw i64 %10, 6
  %13 = add nuw nsw i64 %10, %6
  %14 = getelementptr inbounds double, double* %0, i64 %13
  %15 = load double, double* %14, align 8, !tbaa !2
  %16 = add nuw nsw i64 %12, %8
  %17 = getelementptr inbounds double, double* %1, i64 %16
  %18 = load double, double* %17, align 8, !tbaa !2
  %19 = fmul double %15, %18
  %20 = fadd double %11, %19
  %21 = add nuw nsw i64 %10, 1
  %22 = icmp eq i64 %21, 64
  br i1 %22, label %23, label %9

23:                                               ; preds = %9
  %24 = add nuw nsw i64 %8, %6
  %25 = getelementptr inbounds double, double* %2, i64 %24
  store double %20, double* %25, align 8, !tbaa !2
  %26 = add nuw nsw i64 %8, 1
  %27 = icmp eq i64 %26, 64
  br i1 %27, label %28, label %7

28:                                               ; preds = %23
  %29 = add nuw nsw i64 %5, 1
  %30 = icmp eq i64 %29, 64
  br i1 %30, label %31, label %4

31:                                               ; preds = %28
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
