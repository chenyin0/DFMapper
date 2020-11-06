; ModuleID = 'stencil.c'
source_filename = "stencil.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local i32 @stencil3d(i32* nocapture readonly %0, i32* nocapture readonly %1, i32* nocapture %2) local_unnamed_addr #0 {
  br label %4

4:                                                ; preds = %3, %19
  %5 = phi i64 [ 0, %3 ], [ %20, %19 ]
  %6 = shl i64 %5, 4
  br label %7

7:                                                ; preds = %4, %7
  %8 = phi i64 [ 0, %4 ], [ %17, %7 ]
  %9 = add nuw nsw i64 %8, %6
  %10 = getelementptr inbounds i32, i32* %1, i64 %9
  %11 = load i32, i32* %10, align 4, !tbaa !2
  %12 = getelementptr inbounds i32, i32* %2, i64 %9
  store i32 %11, i32* %12, align 4, !tbaa !2
  %13 = add nuw nsw i64 %9, 15872
  %14 = getelementptr inbounds i32, i32* %1, i64 %13
  %15 = load i32, i32* %14, align 4, !tbaa !2
  %16 = getelementptr inbounds i32, i32* %2, i64 %13
  store i32 %15, i32* %16, align 4, !tbaa !2
  %17 = add nuw nsw i64 %8, 1
  %18 = icmp eq i64 %17, 16
  br i1 %18, label %19, label %7

19:                                               ; preds = %7
  %20 = add nuw nsw i64 %5, 1
  %21 = icmp eq i64 %20, 32
  br i1 %21, label %22, label %4

22:                                               ; preds = %19, %38
  %23 = phi i64 [ %39, %38 ], [ 1, %19 ]
  %24 = shl i64 %23, 9
  %25 = or i64 %24, 496
  br label %26

26:                                               ; preds = %22, %26
  %27 = phi i64 [ 0, %22 ], [ %36, %26 ]
  %28 = add nuw nsw i64 %27, %24
  %29 = getelementptr inbounds i32, i32* %1, i64 %28
  %30 = load i32, i32* %29, align 4, !tbaa !2
  %31 = getelementptr inbounds i32, i32* %2, i64 %28
  store i32 %30, i32* %31, align 4, !tbaa !2
  %32 = add nuw nsw i64 %27, %25
  %33 = getelementptr inbounds i32, i32* %1, i64 %32
  %34 = load i32, i32* %33, align 4, !tbaa !2
  %35 = getelementptr inbounds i32, i32* %2, i64 %32
  store i32 %34, i32* %35, align 4, !tbaa !2
  %36 = add nuw nsw i64 %27, 1
  %37 = icmp eq i64 %36, 16
  br i1 %37, label %38, label %26

38:                                               ; preds = %26
  %39 = add nuw nsw i64 %23, 1
  %40 = icmp eq i64 %39, 31
  br i1 %40, label %41, label %22

41:                                               ; preds = %38, %59
  %42 = phi i64 [ %60, %59 ], [ 1, %38 ]
  %43 = shl nsw i64 %42, 5
  br label %46

44:                                               ; preds = %59
  %45 = getelementptr inbounds i32, i32* %0, i64 1
  br label %62

46:                                               ; preds = %41, %46
  %47 = phi i64 [ 1, %41 ], [ %57, %46 ]
  %48 = add nuw nsw i64 %47, %43
  %49 = shl nsw i64 %48, 4
  %50 = getelementptr inbounds i32, i32* %1, i64 %49
  %51 = load i32, i32* %50, align 4, !tbaa !2
  %52 = getelementptr inbounds i32, i32* %2, i64 %49
  store i32 %51, i32* %52, align 4, !tbaa !2
  %53 = or i64 %49, 15
  %54 = getelementptr inbounds i32, i32* %1, i64 %53
  %55 = load i32, i32* %54, align 4, !tbaa !2
  %56 = getelementptr inbounds i32, i32* %2, i64 %53
  store i32 %55, i32* %56, align 4, !tbaa !2
  %57 = add nuw nsw i64 %47, 1
  %58 = icmp eq i64 %57, 31
  br i1 %58, label %59, label %46

59:                                               ; preds = %46
  %60 = add nuw nsw i64 %42, 1
  %61 = icmp eq i64 %60, 31
  br i1 %61, label %44, label %41

62:                                               ; preds = %44, %120
  %63 = phi i64 [ 1, %44 ], [ %121, %120 ]
  %64 = phi i32 [ undef, %44 ], [ %115, %120 ]
  %65 = phi i32 [ undef, %44 ], [ %109, %120 ]
  %66 = shl i64 %63, 5
  br label %67

67:                                               ; preds = %62, %117
  %68 = phi i64 [ 1, %62 ], [ %118, %117 ]
  %69 = phi i32 [ %64, %62 ], [ %115, %117 ]
  %70 = phi i32 [ %65, %62 ], [ %109, %117 ]
  %71 = add nuw nsw i64 %68, %66
  %72 = shl i64 %71, 4
  %73 = add nuw nsw i64 %72, 512
  %74 = add nsw i64 %72, -512
  %75 = add nuw nsw i64 %72, 16
  %76 = add nsw i64 %72, -16
  br label %77

77:                                               ; preds = %67, %77
  %78 = phi i64 [ 1, %67 ], [ %99, %77 ]
  %79 = phi i32 [ %69, %67 ], [ %115, %77 ]
  %80 = phi i32 [ %70, %67 ], [ %109, %77 ]
  %81 = add nuw nsw i64 %78, %72
  %82 = getelementptr inbounds i32, i32* %1, i64 %81
  %83 = load i32, i32* %82, align 4, !tbaa !2
  %84 = add nuw nsw i64 %78, %73
  %85 = getelementptr inbounds i32, i32* %1, i64 %84
  %86 = load i32, i32* %85, align 4, !tbaa !2
  %87 = add nuw nsw i64 %78, %74
  %88 = getelementptr inbounds i32, i32* %1, i64 %87
  %89 = load i32, i32* %88, align 4, !tbaa !2
  %90 = add nsw i32 %89, %86
  %91 = add nuw nsw i64 %78, %75
  %92 = getelementptr inbounds i32, i32* %1, i64 %91
  %93 = load i32, i32* %92, align 4, !tbaa !2
  %94 = add nsw i32 %90, %93
  %95 = add nuw nsw i64 %78, %76
  %96 = getelementptr inbounds i32, i32* %1, i64 %95
  %97 = load i32, i32* %96, align 4, !tbaa !2
  %98 = add nsw i32 %94, %97
  %99 = add nuw nsw i64 %78, 1
  %100 = add nuw nsw i64 %99, %72
  %101 = getelementptr inbounds i32, i32* %1, i64 %100
  %102 = load i32, i32* %101, align 4, !tbaa !2
  %103 = add nsw i32 %98, %102
  %104 = add nsw i64 %81, -1
  %105 = getelementptr inbounds i32, i32* %1, i64 %104
  %106 = load i32, i32* %105, align 4, !tbaa !2
  %107 = add nsw i32 %103, %106
  %108 = load i32, i32* %0, align 4, !tbaa !2
  %109 = mul nsw i32 %108, %83
  %110 = load i32, i32* %45, align 4, !tbaa !2
  %111 = mul nsw i32 %110, %107
  %112 = add nsw i32 %111, %109
  %113 = getelementptr inbounds i32, i32* %2, i64 %81
  store i32 %112, i32* %113, align 4, !tbaa !2
  %114 = add i32 %80, 1024
  %115 = add i32 %114, %79
  %116 = icmp eq i64 %99, 15
  br i1 %116, label %117, label %77

117:                                              ; preds = %77
  %118 = add nuw nsw i64 %68, 1
  %119 = icmp eq i64 %118, 31
  br i1 %119, label %120, label %67

120:                                              ; preds = %117
  %121 = add nuw nsw i64 %63, 1
  %122 = icmp eq i64 %121, 31
  br i1 %122, label %123, label %62

123:                                              ; preds = %120
  ret i32 %115
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
