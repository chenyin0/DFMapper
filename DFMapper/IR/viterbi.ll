; ModuleID = 'viterbi.c'
source_filename = "viterbi.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local i32 @viterbi(i8* nocapture readonly %0, double* nocapture readonly %1, double* nocapture readonly %2, double* nocapture readonly %3, i8* nocapture %4) local_unnamed_addr #0 {
  %6 = alloca [140 x [64 x double]], align 16
  %7 = bitcast [140 x [64 x double]]* %6 to i8*
  call void @llvm.lifetime.start.p0i8(i64 71680, i8* nonnull %7) #2
  %8 = load i8, i8* %0, align 1, !tbaa !2
  %9 = zext i8 %8 to i64
  br label %10

10:                                               ; preds = %5, %10
  %11 = phi i64 [ 0, %5 ], [ %20, %10 ]
  %12 = getelementptr inbounds double, double* %1, i64 %11
  %13 = load double, double* %12, align 8, !tbaa !5
  %14 = shl nuw nsw i64 %11, 6
  %15 = add nuw nsw i64 %14, %9
  %16 = getelementptr inbounds double, double* %3, i64 %15
  %17 = load double, double* %16, align 8, !tbaa !5
  %18 = fadd double %13, %17
  %19 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 0, i64 %11
  store double %18, double* %19, align 8, !tbaa !5
  %20 = add nuw nsw i64 %11, 1
  %21 = icmp eq i64 %20, 64
  br i1 %21, label %22, label %10

22:                                               ; preds = %10, %60
  %23 = phi i64 [ %61, %60 ], [ 1, %10 ]
  %24 = add nsw i64 %23, -1
  %25 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 %24, i64 0
  %26 = getelementptr inbounds i8, i8* %0, i64 %23
  %27 = load i8, i8* %26, align 1, !tbaa !2
  %28 = zext i8 %27 to i64
  %29 = load double, double* %25, align 16, !tbaa !5
  br label %30

30:                                               ; preds = %22, %56
  %31 = phi i64 [ 0, %22 ], [ %58, %56 ]
  %32 = getelementptr inbounds double, double* %2, i64 %31
  %33 = load double, double* %32, align 8, !tbaa !5
  %34 = fadd double %29, %33
  %35 = shl nuw nsw i64 %31, 6
  %36 = add nuw nsw i64 %35, %28
  %37 = getelementptr inbounds double, double* %3, i64 %36
  %38 = load double, double* %37, align 8, !tbaa !5
  %39 = fadd double %34, %38
  br label %40

40:                                               ; preds = %30, %40
  %41 = phi i64 [ 1, %30 ], [ %54, %40 ]
  %42 = phi double [ %39, %30 ], [ %53, %40 ]
  %43 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 %24, i64 %41
  %44 = load double, double* %43, align 8, !tbaa !5
  %45 = shl i64 %41, 6
  %46 = add i64 %45, %31
  %47 = and i64 %46, 4294967295
  %48 = getelementptr inbounds double, double* %2, i64 %47
  %49 = load double, double* %48, align 8, !tbaa !5
  %50 = fadd double %44, %49
  %51 = fadd double %38, %50
  %52 = fcmp olt double %51, %42
  %53 = select i1 %52, double %51, double %42
  %54 = add nuw nsw i64 %41, 1
  %55 = icmp eq i64 %54, 64
  br i1 %55, label %56, label %40

56:                                               ; preds = %40
  %57 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 %23, i64 %31
  store double %53, double* %57, align 8, !tbaa !5
  %58 = add nuw nsw i64 %31, 1
  %59 = icmp eq i64 %58, 64
  br i1 %59, label %60, label %30

60:                                               ; preds = %56
  %61 = add nuw nsw i64 %23, 1
  %62 = icmp eq i64 %61, 140
  br i1 %62, label %63, label %22

63:                                               ; preds = %60
  %64 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 139, i64 0
  %65 = load double, double* %64, align 16, !tbaa !5
  br label %66

66:                                               ; preds = %63, %66
  %67 = phi i64 [ 1, %63 ], [ %76, %66 ]
  %68 = phi i8 [ 0, %63 ], [ %75, %66 ]
  %69 = phi double [ %65, %63 ], [ %73, %66 ]
  %70 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 139, i64 %67
  %71 = load double, double* %70, align 8, !tbaa !5
  %72 = fcmp olt double %71, %69
  %73 = select i1 %72, double %71, double %69
  %74 = trunc i64 %67 to i8
  %75 = select i1 %72, i8 %74, i8 %68
  %76 = add nuw nsw i64 %67, 1
  %77 = icmp eq i64 %76, 64
  br i1 %77, label %78, label %66

78:                                               ; preds = %66
  %79 = getelementptr inbounds i8, i8* %4, i64 139
  store i8 %75, i8* %79, align 1, !tbaa !2
  br label %80

80:                                               ; preds = %78, %106
  %81 = phi i8 [ %75, %78 ], [ %103, %106 ]
  %82 = phi i64 [ 138, %78 ], [ %108, %106 ]
  %83 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 %82, i64 0
  %84 = load double, double* %83, align 16, !tbaa !5
  %85 = zext i8 %81 to i64
  %86 = getelementptr inbounds double, double* %2, i64 %85
  %87 = load double, double* %86, align 8, !tbaa !5
  %88 = fadd double %84, %87
  br label %89

89:                                               ; preds = %80, %89
  %90 = phi i64 [ 1, %80 ], [ %104, %89 ]
  %91 = phi i8 [ 0, %80 ], [ %103, %89 ]
  %92 = phi double [ %88, %80 ], [ %101, %89 ]
  %93 = getelementptr inbounds [140 x [64 x double]], [140 x [64 x double]]* %6, i64 0, i64 %82, i64 %90
  %94 = load double, double* %93, align 8, !tbaa !5
  %95 = shl nuw nsw i64 %90, 6
  %96 = add nuw nsw i64 %95, %85
  %97 = getelementptr inbounds double, double* %2, i64 %96
  %98 = load double, double* %97, align 8, !tbaa !5
  %99 = fadd double %94, %98
  %100 = fcmp olt double %99, %92
  %101 = select i1 %100, double %99, double %92
  %102 = trunc i64 %90 to i8
  %103 = select i1 %100, i8 %102, i8 %91
  %104 = add nuw nsw i64 %90, 1
  %105 = icmp eq i64 %104, 64
  br i1 %105, label %106, label %89

106:                                              ; preds = %89
  %107 = getelementptr inbounds i8, i8* %4, i64 %82
  store i8 %103, i8* %107, align 1, !tbaa !2
  %108 = add nsw i64 %82, -1
  %109 = icmp eq i64 %82, 0
  br i1 %109, label %110, label %80

110:                                              ; preds = %106
  call void @llvm.lifetime.end.p0i8(i64 71680, i8* nonnull %7) #2
  ret i32 0
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
!5 = !{!6, !6, i64 0}
!6 = !{!"double", !3, i64 0}
