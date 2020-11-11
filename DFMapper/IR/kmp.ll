; ModuleID = 'kmp.c'
source_filename = "kmp.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"


; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local i32 @kmp(i8* nocapture readonly %0, i8* nocapture readonly %1, i32* nocapture %2, i32* nocapture %3) local_unnamed_addr #1 {
  store i32 0, i32* %3, align 4, !tbaa !2
  store i32 0, i32* %2, align 4, !tbaa !2
  br label %5

5:                                                ; preds = %22, %4
  %6 = phi i64 [ 1, %4 ], [ %31, %22 ]
  %7 = phi i32 [ 0, %4 ], [ %29, %22 ]
  %8 = icmp sgt i32 %7, 0
  %9 = getelementptr inbounds i8, i8* %0, i64 %6
  %10 = load i8, i8* %9, align 1, !tbaa !6
  br i1 %8, label %11, label %22

11:                                               ; preds = %5
  %12 = getelementptr inbounds i32, i32* %2, i64 %6
  br label %13

13:                                               ; preds = %19, %11
  %14 = phi i32 [ %7, %11 ], [ %20, %19 ]
  %15 = zext i32 %14 to i64
  %16 = getelementptr inbounds i8, i8* %0, i64 %15
  %17 = load i8, i8* %16, align 1, !tbaa !6
  %18 = icmp eq i8 %17, %10
  br i1 %18, label %22, label %19

19:                                               ; preds = %13
  %20 = load i32, i32* %12, align 4, !tbaa !2
  %21 = icmp sgt i32 %20, 0
  br i1 %21, label %13, label %22

22:                                               ; preds = %19, %13, %5
  %23 = phi i32 [ %7, %5 ], [ %14, %13 ], [ %20, %19 ]
  %24 = sext i32 %23 to i64
  %25 = getelementptr inbounds i8, i8* %0, i64 %24
  %26 = load i8, i8* %25, align 1, !tbaa !6
  %27 = icmp eq i8 %26, %10
  %28 = zext i1 %27 to i32
  %29 = add nsw i32 %23, %28
  %30 = getelementptr inbounds i32, i32* %2, i64 %6
  store i32 %29, i32* %30, align 4, !tbaa !2
  %31 = add nuw nsw i64 %6, 1
  %32 = icmp eq i64 %31, 4
  br i1 %32, label %33, label %5

33:                                               ; preds = %22, %65
  %34 = phi i64 [ %67, %65 ], [ 0, %22 ]
  %35 = phi i32 [ %66, %65 ], [ 0, %22 ]
  %36 = icmp sgt i32 %35, 0
  %37 = getelementptr inbounds i8, i8* %1, i64 %34
  %38 = load i8, i8* %37, align 1, !tbaa !6
  br i1 %36, label %39, label %49

39:                                               ; preds = %33, %45
  %40 = phi i32 [ %47, %45 ], [ %35, %33 ]
  %41 = zext i32 %40 to i64
  %42 = getelementptr inbounds i8, i8* %0, i64 %41
  %43 = load i8, i8* %42, align 1, !tbaa !6
  %44 = icmp eq i8 %43, %38
  br i1 %44, label %49, label %45

45:                                               ; preds = %39
  %46 = getelementptr inbounds i32, i32* %2, i64 %41
  %47 = load i32, i32* %46, align 4, !tbaa !2
  %48 = icmp sgt i32 %47, 0
  br i1 %48, label %39, label %49

49:                                               ; preds = %39, %45, %33
  %50 = phi i32 [ %35, %33 ], [ %47, %45 ], [ %40, %39 ]
  %51 = sext i32 %50 to i64
  %52 = getelementptr inbounds i8, i8* %0, i64 %51
  %53 = load i8, i8* %52, align 1, !tbaa !6
  %54 = icmp eq i8 %53, %38
  %55 = zext i1 %54 to i32
  %56 = add nsw i32 %50, %55
  %57 = icmp sgt i32 %56, 3
  br i1 %57, label %58, label %65

58:                                               ; preds = %49
  %59 = load i32, i32* %3, align 4, !tbaa !2
  %60 = add nsw i32 %59, 1
  store i32 %60, i32* %3, align 4, !tbaa !2
  %61 = add nsw i32 %56, -1
  %62 = zext i32 %61 to i64
  %63 = getelementptr inbounds i32, i32* %2, i64 %62
  %64 = load i32, i32* %63, align 4, !tbaa !2
  br label %65

65:                                               ; preds = %49, %58
  %66 = phi i32 [ %64, %58 ], [ %56, %49 ]
  %67 = add nuw nsw i64 %34, 1
  %68 = icmp eq i64 %67, 32411
  br i1 %68, label %69, label %33

69:                                               ; preds = %65
  ret i32 0
}

attributes #0 = { alwaysinline nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!4, !4, i64 0}
