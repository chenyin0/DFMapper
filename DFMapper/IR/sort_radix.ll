; ModuleID = 'sort.c'
source_filename = "sort.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @ss_sort(i32* nocapture %0, i32* nocapture %1, i32* nocapture %2, i32* nocapture %3) local_unnamed_addr #2 {
  %5 = bitcast i32* %2 to i8*
  br label %6

6:                                                ; preds = %4, %152
  %7 = phi i32 [ 0, %4 ], [ %153, %152 ]
  %8 = phi i32 [ 0, %4 ], [ %154, %152 ]
  tail call void @llvm.memset.p0i8.i64(i8* nonnull align 4 dereferenceable(8192) %5, i8 0, i64 8192, i1 false) #4
  %9 = icmp eq i32 %7, 0
  br i1 %9, label %10, label %32

10:                                               ; preds = %6, %30
  %11 = phi i64 [ %13, %30 ], [ 0, %6 ]
  %12 = shl nsw i64 %11, 2
  %13 = add nuw nsw i64 %11, 1
  %14 = trunc i64 %13 to i32
  br label %15

15:                                               ; preds = %15, %10
  %16 = phi i64 [ 0, %10 ], [ %28, %15 ]
  %17 = add nuw nsw i64 %16, %12
  %18 = getelementptr inbounds i32, i32* %0, i64 %17
  %19 = load i32, i32* %18, align 4, !tbaa !2
  %20 = ashr i32 %19, %8
  %21 = shl i32 %20, 9
  %22 = and i32 %21, 1536
  %23 = add i32 %22, %14
  %24 = zext i32 %23 to i64
  %25 = getelementptr inbounds i32, i32* %2, i64 %24
  %26 = load i32, i32* %25, align 4, !tbaa !2
  %27 = add nsw i32 %26, 1
  store i32 %27, i32* %25, align 4, !tbaa !2
  %28 = add nuw nsw i64 %16, 1
  %29 = icmp eq i64 %28, 4
  br i1 %29, label %30, label %15

30:                                               ; preds = %15
  %31 = icmp eq i64 %13, 512
  br i1 %31, label %54, label %10

32:                                               ; preds = %6, %52
  %33 = phi i64 [ %35, %52 ], [ 0, %6 ]
  %34 = shl nsw i64 %33, 2
  %35 = add nuw nsw i64 %33, 1
  %36 = trunc i64 %35 to i32
  br label %37

37:                                               ; preds = %37, %32
  %38 = phi i64 [ 0, %32 ], [ %50, %37 ]
  %39 = add nuw nsw i64 %38, %34
  %40 = getelementptr inbounds i32, i32* %1, i64 %39
  %41 = load i32, i32* %40, align 4, !tbaa !2
  %42 = ashr i32 %41, %8
  %43 = shl i32 %42, 9
  %44 = and i32 %43, 1536
  %45 = add i32 %44, %36
  %46 = zext i32 %45 to i64
  %47 = getelementptr inbounds i32, i32* %2, i64 %46
  %48 = load i32, i32* %47, align 4, !tbaa !2
  %49 = add nsw i32 %48, 1
  store i32 %49, i32* %47, align 4, !tbaa !2
  %50 = add nuw nsw i64 %38, 1
  %51 = icmp eq i64 %50, 4
  br i1 %51, label %52, label %37

52:                                               ; preds = %37
  %53 = icmp eq i64 %35, 512
  br i1 %53, label %54, label %32

54:                                               ; preds = %52, %30
  br label %55

55:                                               ; preds = %54, %70
  %56 = phi i64 [ %71, %70 ], [ 0, %54 ]
  %57 = shl nuw nsw i64 %56, 4
  %58 = getelementptr i32, i32* %2, i64 %57
  %59 = shl nsw i64 %56, 4
  %60 = load i32, i32* %58, align 4
  br label %61

61:                                               ; preds = %61, %55
  %62 = phi i32 [ %60, %55 ], [ %67, %61 ]
  %63 = phi i64 [ 1, %55 ], [ %68, %61 ]
  %64 = add nuw nsw i64 %63, %59
  %65 = getelementptr inbounds i32, i32* %2, i64 %64
  %66 = load i32, i32* %65, align 4, !tbaa !2
  %67 = add nsw i32 %66, %62
  store i32 %67, i32* %65, align 4, !tbaa !2
  %68 = add nuw nsw i64 %63, 1
  %69 = icmp eq i64 %68, 16
  br i1 %69, label %70, label %61

70:                                               ; preds = %61
  %71 = add nuw nsw i64 %56, 1
  %72 = icmp eq i64 %71, 128
  br i1 %72, label %73, label %55

73:                                               ; preds = %70
  store i32 0, i32* %3, align 4, !tbaa !2
  br label %74

74:                                               ; preds = %74, %73
  %75 = phi i32 [ 0, %73 ], [ %81, %74 ]
  %76 = phi i64 [ 1, %73 ], [ %83, %74 ]
  %77 = shl nuw nsw i64 %76, 4
  %78 = add nsw i64 %77, -1
  %79 = getelementptr inbounds i32, i32* %2, i64 %78
  %80 = load i32, i32* %79, align 4, !tbaa !2
  %81 = add nsw i32 %80, %75
  %82 = getelementptr inbounds i32, i32* %3, i64 %76
  store i32 %81, i32* %82, align 4, !tbaa !2
  %83 = add nuw nsw i64 %76, 1
  %84 = icmp eq i64 %83, 128
  br i1 %84, label %85, label %74

85:                                               ; preds = %74, %98
  %86 = phi i64 [ %99, %98 ], [ 0, %74 ]
  %87 = shl nsw i64 %86, 4
  %88 = getelementptr inbounds i32, i32* %3, i64 %86
  br label %89

89:                                               ; preds = %89, %85
  %90 = phi i64 [ 0, %85 ], [ %96, %89 ]
  %91 = add nuw nsw i64 %90, %87
  %92 = getelementptr inbounds i32, i32* %2, i64 %91
  %93 = load i32, i32* %92, align 4, !tbaa !2
  %94 = load i32, i32* %88, align 4, !tbaa !2
  %95 = add nsw i32 %94, %93
  store i32 %95, i32* %92, align 4, !tbaa !2
  %96 = add nuw nsw i64 %90, 1
  %97 = icmp eq i64 %96, 16
  br i1 %97, label %98, label %89

98:                                               ; preds = %89
  %99 = add nuw nsw i64 %86, 1
  %100 = icmp eq i64 %99, 128
  br i1 %100, label %101, label %85

101:                                              ; preds = %98
  br i1 %9, label %102, label %127

102:                                              ; preds = %101, %124
  %103 = phi i64 [ %125, %124 ], [ 0, %101 ]
  %104 = shl nsw i64 %103, 2
  %105 = trunc i64 %103 to i32
  br label %106

106:                                              ; preds = %106, %102
  %107 = phi i64 [ 0, %102 ], [ %122, %106 ]
  %108 = add nuw nsw i64 %107, %104
  %109 = getelementptr inbounds i32, i32* %0, i64 %108
  %110 = load i32, i32* %109, align 4, !tbaa !2
  %111 = ashr i32 %110, %8
  %112 = shl i32 %111, 9
  %113 = and i32 %112, 1536
  %114 = add nuw nsw i32 %113, %105
  %115 = zext i32 %114 to i64
  %116 = getelementptr inbounds i32, i32* %2, i64 %115
  %117 = load i32, i32* %116, align 4, !tbaa !2
  %118 = sext i32 %117 to i64
  %119 = getelementptr inbounds i32, i32* %1, i64 %118
  store i32 %110, i32* %119, align 4, !tbaa !2
  %120 = load i32, i32* %116, align 4, !tbaa !2
  %121 = add nsw i32 %120, 1
  store i32 %121, i32* %116, align 4, !tbaa !2
  %122 = add nuw nsw i64 %107, 1
  %123 = icmp eq i64 %122, 4
  br i1 %123, label %124, label %106

124:                                              ; preds = %106
  %125 = add nuw nsw i64 %103, 1
  %126 = icmp eq i64 %125, 512
  br i1 %126, label %152, label %102

127:                                              ; preds = %101, %149
  %128 = phi i64 [ %150, %149 ], [ 0, %101 ]
  %129 = shl nsw i64 %128, 2
  %130 = trunc i64 %128 to i32
  br label %131

131:                                              ; preds = %131, %127
  %132 = phi i64 [ 0, %127 ], [ %147, %131 ]
  %133 = add nuw nsw i64 %132, %129
  %134 = getelementptr inbounds i32, i32* %1, i64 %133
  %135 = load i32, i32* %134, align 4, !tbaa !2
  %136 = ashr i32 %135, %8
  %137 = shl i32 %136, 9
  %138 = and i32 %137, 1536
  %139 = add nuw nsw i32 %138, %130
  %140 = zext i32 %139 to i64
  %141 = getelementptr inbounds i32, i32* %2, i64 %140
  %142 = load i32, i32* %141, align 4, !tbaa !2
  %143 = sext i32 %142 to i64
  %144 = getelementptr inbounds i32, i32* %0, i64 %143
  store i32 %135, i32* %144, align 4, !tbaa !2
  %145 = load i32, i32* %141, align 4, !tbaa !2
  %146 = add nsw i32 %145, 1
  store i32 %146, i32* %141, align 4, !tbaa !2
  %147 = add nuw nsw i64 %132, 1
  %148 = icmp eq i64 %147, 4
  br i1 %148, label %149, label %131

149:                                              ; preds = %131
  %150 = add nuw nsw i64 %128, 1
  %151 = icmp eq i64 %150, 512
  br i1 %151, label %152, label %127

152:                                              ; preds = %149, %124
  %153 = phi i32 [ 1, %124 ], [ 0, %149 ]
  %154 = add nuw nsw i32 %8, 2
  %155 = icmp ult i32 %8, 30
  br i1 %155, label %6, label %156

156:                                              ; preds = %152
  ret void
}

; Function Attrs: argmemonly nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #3

attributes #0 = { alwaysinline nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { alwaysinline nofree norecurse nounwind optsize uwtable writeonly "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind willreturn writeonly }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
