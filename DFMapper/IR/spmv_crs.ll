; ModuleID = 'spmv.c'
source_filename = "spmv.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @spmv(double* nocapture readonly %0, i32* nocapture readonly %1, i32* nocapture readonly %2, double* nocapture readonly %3, double* nocapture %4) local_unnamed_addr #0 {
  %6 = load i32, i32* %2, align 4, !tbaa !2
  br label %7

7:                                                ; preds = %5, %31
  %8 = phi i32 [ %6, %5 ], [ %12, %31 ]
  %9 = phi i64 [ 0, %5 ], [ %10, %31 ]
  %10 = add nuw nsw i64 %9, 1
  %11 = getelementptr inbounds i32, i32* %2, i64 %10
  %12 = load i32, i32* %11, align 4, !tbaa !2
  %13 = icmp slt i32 %8, %12
  br i1 %13, label %14, label %31

14:                                               ; preds = %7
  %15 = sext i32 %8 to i64
  %16 = sext i32 %12 to i64
  br label %17

17:                                               ; preds = %14, %17
  %18 = phi i64 [ %15, %14 ], [ %29, %17 ]
  %19 = phi double [ 0.000000e+00, %14 ], [ %28, %17 ]
  %20 = getelementptr inbounds double, double* %0, i64 %18
  %21 = load double, double* %20, align 8, !tbaa !6
  %22 = getelementptr inbounds i32, i32* %1, i64 %18
  %23 = load i32, i32* %22, align 4, !tbaa !2
  %24 = sext i32 %23 to i64
  %25 = getelementptr inbounds double, double* %3, i64 %24
  %26 = load double, double* %25, align 8, !tbaa !6
  %27 = fmul double %21, %26
  %28 = fadd double %19, %27
  %29 = add nsw i64 %18, 1
  %30 = icmp eq i64 %29, %16
  br i1 %30, label %31, label %17

31:                                               ; preds = %17, %7
  %32 = phi double [ 0.000000e+00, %7 ], [ %28, %17 ]
  %33 = getelementptr inbounds double, double* %4, i64 %9
  store double %32, double* %33, align 8, !tbaa !6
  %34 = icmp eq i64 %10, 494
  br i1 %34, label %35, label %7

35:                                               ; preds = %31
  ret void
}

attributes #0 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"double", !4, i64 0}
