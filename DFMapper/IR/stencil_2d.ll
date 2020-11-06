; ModuleID = 'stencil.c'
source_filename = "stencil.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local i32 @stencil(i32* nocapture readonly %0, i32* nocapture %1, i32* nocapture readonly %2) local_unnamed_addr #0 {
  br label %4

4:                                                ; preds = %3, %47
  %5 = phi i64 [ 0, %3 ], [ %48, %47 ]
  %6 = phi i32 [ undef, %3 ], [ %36, %47 ]
  %7 = phi i32 [ undef, %3 ], [ %33, %47 ]
  %8 = shl nsw i64 %5, 6
  br label %9

9:                                                ; preds = %4, %42
  %10 = phi i64 [ 0, %4 ], [ %45, %42 ]
  %11 = phi i32 [ %6, %4 ], [ %36, %42 ]
  %12 = phi i32 [ %7, %4 ], [ %33, %42 ]
  br label %13

13:                                               ; preds = %9, %39
  %14 = phi i64 [ 0, %9 ], [ %40, %39 ]
  %15 = phi i32 [ %11, %9 ], [ %36, %39 ]
  %16 = phi i32 [ %12, %9 ], [ %33, %39 ]
  %17 = phi i32 [ 0, %9 ], [ %34, %39 ]
  %18 = mul nuw nsw i64 %14, 3
  %19 = add nuw nsw i64 %14, %5
  %20 = shl nsw i64 %19, 6
  %21 = add nuw nsw i64 %20, %10
  br label %22

22:                                               ; preds = %13, %22
  %23 = phi i64 [ 0, %13 ], [ %37, %22 ]
  %24 = phi i32 [ %15, %13 ], [ %36, %22 ]
  %25 = phi i32 [ %16, %13 ], [ %33, %22 ]
  %26 = phi i32 [ %17, %13 ], [ %34, %22 ]
  %27 = add nuw nsw i64 %23, %18
  %28 = getelementptr inbounds i32, i32* %2, i64 %27
  %29 = load i32, i32* %28, align 4, !tbaa !2
  %30 = add nuw nsw i64 %21, %23
  %31 = getelementptr inbounds i32, i32* %0, i64 %30
  %32 = load i32, i32* %31, align 4, !tbaa !2
  %33 = mul nsw i32 %32, %29
  %34 = add nsw i32 %33, %26
  %35 = add i32 %25, 1024
  %36 = add i32 %35, %24
  %37 = add nuw nsw i64 %23, 1
  %38 = icmp eq i64 %37, 3
  br i1 %38, label %39, label %22

39:                                               ; preds = %22
  %40 = add nuw nsw i64 %14, 1
  %41 = icmp eq i64 %40, 3
  br i1 %41, label %42, label %13

42:                                               ; preds = %39
  %43 = add nuw nsw i64 %10, %8
  %44 = getelementptr inbounds i32, i32* %1, i64 %43
  store i32 %34, i32* %44, align 4, !tbaa !2
  %45 = add nuw nsw i64 %10, 1
  %46 = icmp eq i64 %45, 62
  br i1 %46, label %47, label %9

47:                                               ; preds = %42
  %48 = add nuw nsw i64 %5, 1
  %49 = icmp eq i64 %48, 126
  br i1 %49, label %50, label %4

50:                                               ; preds = %47
  ret i32 %36
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
