; ModuleID = 'sort.c'
source_filename = "sort.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"


; Function Attrs: nounwind optsize uwtable
define dso_local void @ms_mergesort(i32* nocapture %0) local_unnamed_addr #2 {
  %2 = alloca [2048 x i32], align 16
  %3 = bitcast [2048 x i32]* %2 to i8*
  br label %4

4:                                                ; preds = %1, %124
  %5 = phi i32 [ 1, %1 ], [ %7, %124 ]
  %6 = icmp sgt i32 %5, 0
  %7 = shl i32 %5, 1
  %8 = add i32 %5, -1
  %9 = sext i32 %7 to i64
  %10 = sext i32 %5 to i64
  %11 = zext i32 %8 to i64
  %12 = shl nuw nsw i64 %11, 2
  %13 = add nuw nsw i64 %12, 4
  %14 = shl i32 %5, 1
  %15 = zext i32 %8 to i64
  %16 = shl nuw nsw i64 %15, 2
  %17 = add nuw nsw i64 %16, 4
  br label %18

18:                                               ; preds = %4, %120
  %19 = phi i64 [ 0, %4 ], [ %121, %120 ]
  %20 = phi i32 [ 0, %4 ], [ %122, %120 ]
  %21 = add nsw i64 %19, %10
  %22 = add nsw i64 %21, %10
  %23 = icmp slt i64 %22, 2049
  call void @llvm.lifetime.start.p0i8(i64 8192, i8* nonnull %3) #3
  br i1 %23, label %24, label %75

24:                                               ; preds = %18
  br i1 %6, label %25, label %35

25:                                               ; preds = %24
  %26 = add i32 %20, %14
  %27 = add nsw i32 %26, -1
  %28 = getelementptr [2048 x i32], [2048 x i32]* %2, i64 0, i64 %19
  %29 = bitcast i32* %28 to i8*
  %30 = getelementptr i32, i32* %0, i64 %19
  %31 = bitcast i32* %30 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 dereferenceable(1) %29, i8* nonnull align 4 dereferenceable(1) %31, i64 %17, i1 false) #3
  %32 = trunc i64 %21 to i32
  %33 = add nsw i32 %27, %32
  %34 = trunc i64 %22 to i32
  br label %42

35:                                               ; preds = %42, %24
  %36 = icmp slt i64 %19, %22
  br i1 %36, label %37, label %74

37:                                               ; preds = %35
  %38 = trunc i64 %19 to i32
  %39 = trunc i64 %22 to i32
  %40 = add i32 %39, -1
  %41 = trunc i64 %22 to i32
  br label %53

42:                                               ; preds = %42, %25
  %43 = phi i64 [ %21, %25 ], [ %50, %42 ]
  %44 = getelementptr inbounds i32, i32* %0, i64 %43
  %45 = load i32, i32* %44, align 4, !tbaa !2
  %46 = trunc i64 %43 to i32
  %47 = sub i32 %33, %46
  %48 = sext i32 %47 to i64
  %49 = getelementptr inbounds [2048 x i32], [2048 x i32]* %2, i64 0, i64 %48
  store i32 %45, i32* %49, align 4, !tbaa !2
  %50 = add nsw i64 %43, 1
  %51 = trunc i64 %50 to i32
  %52 = icmp eq i32 %34, %51
  br i1 %52, label %35, label %42

53:                                               ; preds = %53, %37
  %54 = phi i64 [ %19, %37 ], [ %71, %53 ]
  %55 = phi i32 [ %38, %37 ], [ %69, %53 ]
  %56 = phi i32 [ %40, %37 ], [ %66, %53 ]
  %57 = sext i32 %56 to i64
  %58 = getelementptr inbounds [2048 x i32], [2048 x i32]* %2, i64 0, i64 %57
  %59 = load i32, i32* %58, align 4, !tbaa !2
  %60 = sext i32 %55 to i64
  %61 = getelementptr inbounds [2048 x i32], [2048 x i32]* %2, i64 0, i64 %60
  %62 = load i32, i32* %61, align 4, !tbaa !2
  %63 = icmp slt i32 %59, %62
  %64 = select i1 %63, i32 %59, i32 %62
  %65 = sext i1 %63 to i32
  %66 = add nsw i32 %56, %65
  %67 = xor i1 %63, true
  %68 = zext i1 %67 to i32
  %69 = add nsw i32 %55, %68
  %70 = getelementptr inbounds i32, i32* %0, i64 %54
  store i32 %64, i32* %70, align 4
  %71 = add nsw i64 %54, 1
  %72 = trunc i64 %71 to i32
  %73 = icmp eq i32 %41, %72
  br i1 %73, label %74, label %53

74:                                               ; preds = %53, %35
  call void @llvm.lifetime.end.p0i8(i64 8192, i8* nonnull %3) #3
  br label %120

75:                                               ; preds = %18
  br i1 %6, label %76, label %81

76:                                               ; preds = %75
  %77 = getelementptr [2048 x i32], [2048 x i32]* %2, i64 0, i64 %19
  %78 = bitcast i32* %77 to i8*
  %79 = getelementptr i32, i32* %0, i64 %19
  %80 = bitcast i32* %79 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 dereferenceable(1) %78, i8* nonnull align 4 dereferenceable(1) %80, i64 %13, i1 false) #3
  br label %81

81:                                               ; preds = %76, %75
  %82 = icmp slt i64 %21, 2049
  br i1 %82, label %83, label %85

83:                                               ; preds = %81
  %84 = add i64 %21, 2048
  br label %87

85:                                               ; preds = %87, %81
  %86 = trunc i64 %19 to i32
  br label %98

87:                                               ; preds = %87, %83
  %88 = phi i64 [ %21, %83 ], [ %95, %87 ]
  %89 = getelementptr inbounds i32, i32* %0, i64 %88
  %90 = load i32, i32* %89, align 4, !tbaa !2
  %91 = sub i64 %84, %88
  %92 = shl i64 %91, 32
  %93 = ashr exact i64 %92, 32
  %94 = getelementptr inbounds [2048 x i32], [2048 x i32]* %2, i64 0, i64 %93
  store i32 %90, i32* %94, align 4, !tbaa !2
  %95 = add nsw i64 %88, 1
  %96 = trunc i64 %95 to i32
  %97 = icmp eq i32 %96, 2049
  br i1 %97, label %85, label %87

98:                                               ; preds = %98, %85
  %99 = phi i64 [ %19, %85 ], [ %116, %98 ]
  %100 = phi i32 [ %86, %85 ], [ %114, %98 ]
  %101 = phi i32 [ 2048, %85 ], [ %111, %98 ]
  %102 = sext i32 %101 to i64
  %103 = getelementptr inbounds [2048 x i32], [2048 x i32]* %2, i64 0, i64 %102
  %104 = load i32, i32* %103, align 4, !tbaa !2
  %105 = sext i32 %100 to i64
  %106 = getelementptr inbounds [2048 x i32], [2048 x i32]* %2, i64 0, i64 %105
  %107 = load i32, i32* %106, align 4, !tbaa !2
  %108 = icmp slt i32 %104, %107
  %109 = select i1 %108, i32 %104, i32 %107
  %110 = sext i1 %108 to i32
  %111 = add nsw i32 %101, %110
  %112 = xor i1 %108, true
  %113 = zext i1 %112 to i32
  %114 = add nsw i32 %100, %113
  %115 = getelementptr inbounds i32, i32* %0, i64 %99
  store i32 %109, i32* %115, align 4
  %116 = add nsw i64 %99, 1
  %117 = trunc i64 %116 to i32
  %118 = icmp eq i32 %117, 2049
  br i1 %118, label %119, label %98

119:                                              ; preds = %98
  call void @llvm.lifetime.end.p0i8(i64 8192, i8* nonnull %3) #3
  br label %120

120:                                              ; preds = %74, %119
  %121 = add nsw i64 %19, %9
  %122 = add nsw i32 %20, %7
  %123 = icmp slt i64 %121, 2048
  br i1 %123, label %18, label %124

124:                                              ; preds = %120
  %125 = icmp slt i32 %5, 1024
  br i1 %125, label %4, label %126

126:                                              ; preds = %124
  ret void
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { alwaysinline nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
