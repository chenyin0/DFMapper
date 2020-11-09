; ModuleID = 'spmv.c'
source_filename = "spmv.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @ellpack(double* nocapture readonly %0, i32* nocapture readonly %1, double* nocapture readonly %2, double* nocapture %3) local_unnamed_addr #0 {
  br label %5

5:                                                ; preds = %4, %25
  %6 = phi i64 [ 0, %4 ], [ %26, %25 ]
  %7 = getelementptr inbounds double, double* %3, i64 %6
  %8 = load double, double* %7, align 8, !tbaa !2
  %9 = mul nuw nsw i64 %6, 10
  br label %10

10:                                               ; preds = %5, %10
  %11 = phi i64 [ 0, %5 ], [ %23, %10 ]
  %12 = phi double [ %8, %5 ], [ %22, %10 ]
  %13 = add nuw nsw i64 %11, %9
  %14 = getelementptr inbounds double, double* %0, i64 %13
  %15 = load double, double* %14, align 8, !tbaa !2
  %16 = getelementptr inbounds i32, i32* %1, i64 %13
  %17 = load i32, i32* %16, align 4, !tbaa !6
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds double, double* %2, i64 %18
  %20 = load double, double* %19, align 8, !tbaa !2
  %21 = fmul double %15, %20
  %22 = fadd double %12, %21
  %23 = add nuw nsw i64 %11, 1
  %24 = icmp eq i64 %23, 10
  br i1 %24, label %25, label %10

25:                                               ; preds = %10
  store double %22, double* %7, align 8, !tbaa !2
  %26 = add nuw nsw i64 %6, 1
  %27 = icmp eq i64 %26, 494
  br i1 %27, label %28, label %5

28:                                               ; preds = %25
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
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !4, i64 0}
