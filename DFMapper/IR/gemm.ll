; ModuleID = 'gemm.c'
source_filename = "gemm.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local i64 @bbgemm(double* nocapture readonly %0, double* nocapture readonly %1, double* nocapture %2) local_unnamed_addr #0 {
  br label %4

4:                                                ; preds = %3, %71
  %5 = phi i64 [ 0, %3 ], [ %72, %71 ]
  %6 = phi double [ undef, %3 ], [ %148, %71 ]
  %7 = phi i32 [ undef, %3 ], [ %153, %71 ]
  %8 = phi i32 [ undef, %3 ], [ %157, %71 ]
  br label %9

9:                                                ; preds = %4, %67
  %10 = phi i64 [ 0, %4 ], [ %69, %67 ]
  %11 = phi double [ %6, %4 ], [ %148, %67 ]
  %12 = phi i32 [ %7, %4 ], [ %153, %67 ]
  %13 = phi i32 [ 4032, %4 ], [ %68, %67 ]
  %14 = phi i32 [ %8, %4 ], [ %157, %67 ]
  br label %15

15:                                               ; preds = %9, %63
  %16 = phi i64 [ 0, %9 ], [ %65, %63 ]
  %17 = phi double [ %11, %9 ], [ %148, %63 ]
  %18 = phi i32 [ %12, %9 ], [ %153, %63 ]
  %19 = phi i32 [ %13, %9 ], [ %64, %63 ]
  %20 = phi i32 [ %14, %9 ], [ %157, %63 ]
  %21 = shl nsw i64 %16, 6
  br label %22

22:                                               ; preds = %156, %15
  %23 = phi i64 [ 0, %15 ], [ %158, %156 ]
  %24 = phi double [ %17, %15 ], [ %148, %156 ]
  %25 = phi i32 [ %18, %15 ], [ %153, %156 ]
  %26 = phi i32 [ %19, %15 ], [ %159, %156 ]
  %27 = phi i32 [ %20, %15 ], [ %157, %156 ]
  %28 = add nuw nsw i64 %23, %10
  %29 = shl nsw i64 %28, 6
  %30 = add nuw nsw i64 %28, %21
  %31 = getelementptr inbounds double, double* %0, i64 %30
  %32 = load double, double* %31, align 8, !tbaa !2
  %33 = add i32 %27, 1024
  br label %34

34:                                               ; preds = %22, %34
  %35 = phi i64 [ 0, %22 ], [ %51, %34 ]
  %36 = phi double [ %24, %22 ], [ %45, %34 ]
  %37 = phi i32 [ %25, %22 ], [ %50, %34 ]
  %38 = sitofp i32 %37 to double
  %39 = fadd double %36, %38
  %40 = fptosi double %39 to i32
  %41 = add nuw nsw i64 %35, %5
  %42 = add nuw nsw i64 %41, %29
  %43 = getelementptr inbounds double, double* %1, i64 %42
  %44 = load double, double* %43, align 8, !tbaa !2
  %45 = fmul double %32, %44
  %46 = add nuw nsw i64 %41, %21
  %47 = getelementptr inbounds double, double* %2, i64 %46
  %48 = load double, double* %47, align 8, !tbaa !2
  %49 = fadd double %48, %45
  store double %49, double* %47, align 8, !tbaa !2
  %50 = add nsw i32 %40, 1024
  %51 = add nuw nsw i64 %35, 1
  %52 = icmp eq i64 %51, 8
  br i1 %52, label %53, label %34

53:                                               ; preds = %34
  %54 = add i32 %33, %26
  %55 = or i64 %23, 1
  %56 = trunc i64 %29 to i32
  %57 = add nuw nsw i64 %55, %10
  %58 = shl nsw i64 %57, 6
  %59 = add nuw nsw i64 %57, %21
  %60 = getelementptr inbounds double, double* %0, i64 %59
  %61 = load double, double* %60, align 8, !tbaa !2
  %62 = add i32 %54, 1024
  br label %79

63:                                               ; preds = %156
  %64 = trunc i64 %132 to i32
  %65 = add nuw nsw i64 %16, 1
  %66 = icmp eq i64 %65, 64
  br i1 %66, label %67, label %15

67:                                               ; preds = %63
  %68 = trunc i64 %132 to i32
  %69 = add nuw nsw i64 %10, 8
  %70 = icmp ult i64 %10, 56
  br i1 %70, label %9, label %71

71:                                               ; preds = %67
  %72 = add nuw nsw i64 %5, 8
  %73 = icmp ult i64 %5, 56
  br i1 %73, label %4, label %74

74:                                               ; preds = %71
  %75 = zext i32 %153 to i64
  %76 = shl nuw i64 %75, 32
  %77 = zext i32 %157 to i64
  %78 = or i64 %76, %77
  ret i64 %78

79:                                               ; preds = %79, %53
  %80 = phi i64 [ 0, %53 ], [ %96, %79 ]
  %81 = phi double [ %45, %53 ], [ %90, %79 ]
  %82 = phi i32 [ %50, %53 ], [ %95, %79 ]
  %83 = sitofp i32 %82 to double
  %84 = fadd double %81, %83
  %85 = fptosi double %84 to i32
  %86 = add nuw nsw i64 %80, %5
  %87 = add nuw nsw i64 %86, %58
  %88 = getelementptr inbounds double, double* %1, i64 %87
  %89 = load double, double* %88, align 8, !tbaa !2
  %90 = fmul double %61, %89
  %91 = add nuw nsw i64 %86, %21
  %92 = getelementptr inbounds double, double* %2, i64 %91
  %93 = load double, double* %92, align 8, !tbaa !2
  %94 = fadd double %93, %90
  store double %94, double* %92, align 8, !tbaa !2
  %95 = add nsw i32 %85, 1024
  %96 = add nuw nsw i64 %80, 1
  %97 = icmp eq i64 %96, 8
  br i1 %97, label %98, label %79

98:                                               ; preds = %79
  %99 = add i32 %62, %56
  %100 = or i64 %23, 2
  %101 = trunc i64 %58 to i32
  %102 = add nuw nsw i64 %100, %10
  %103 = shl nsw i64 %102, 6
  %104 = add nuw nsw i64 %102, %21
  %105 = getelementptr inbounds double, double* %0, i64 %104
  %106 = load double, double* %105, align 8, !tbaa !2
  %107 = add i32 %99, 1024
  br label %108

108:                                              ; preds = %108, %98
  %109 = phi i64 [ 0, %98 ], [ %125, %108 ]
  %110 = phi double [ %90, %98 ], [ %119, %108 ]
  %111 = phi i32 [ %95, %98 ], [ %124, %108 ]
  %112 = sitofp i32 %111 to double
  %113 = fadd double %110, %112
  %114 = fptosi double %113 to i32
  %115 = add nuw nsw i64 %109, %5
  %116 = add nuw nsw i64 %115, %103
  %117 = getelementptr inbounds double, double* %1, i64 %116
  %118 = load double, double* %117, align 8, !tbaa !2
  %119 = fmul double %106, %118
  %120 = add nuw nsw i64 %115, %21
  %121 = getelementptr inbounds double, double* %2, i64 %120
  %122 = load double, double* %121, align 8, !tbaa !2
  %123 = fadd double %122, %119
  store double %123, double* %121, align 8, !tbaa !2
  %124 = add nsw i32 %114, 1024
  %125 = add nuw nsw i64 %109, 1
  %126 = icmp eq i64 %125, 8
  br i1 %126, label %127, label %108

127:                                              ; preds = %108
  %128 = add i32 %107, %101
  %129 = or i64 %23, 3
  %130 = trunc i64 %103 to i32
  %131 = add nuw nsw i64 %129, %10
  %132 = shl nsw i64 %131, 6
  %133 = add nuw nsw i64 %131, %21
  %134 = getelementptr inbounds double, double* %0, i64 %133
  %135 = load double, double* %134, align 8, !tbaa !2
  %136 = add i32 %128, 1024
  br label %137

137:                                              ; preds = %137, %127
  %138 = phi i64 [ 0, %127 ], [ %154, %137 ]
  %139 = phi double [ %119, %127 ], [ %148, %137 ]
  %140 = phi i32 [ %124, %127 ], [ %153, %137 ]
  %141 = sitofp i32 %140 to double
  %142 = fadd double %139, %141
  %143 = fptosi double %142 to i32
  %144 = add nuw nsw i64 %138, %5
  %145 = add nuw nsw i64 %144, %132
  %146 = getelementptr inbounds double, double* %1, i64 %145
  %147 = load double, double* %146, align 8, !tbaa !2
  %148 = fmul double %135, %147
  %149 = add nuw nsw i64 %144, %21
  %150 = getelementptr inbounds double, double* %2, i64 %149
  %151 = load double, double* %150, align 8, !tbaa !2
  %152 = fadd double %151, %148
  store double %152, double* %150, align 8, !tbaa !2
  %153 = add nsw i32 %143, 1024
  %154 = add nuw nsw i64 %138, 1
  %155 = icmp eq i64 %154, 8
  br i1 %155, label %156, label %137

156:                                              ; preds = %137
  %157 = add i32 %136, %130
  %158 = add nuw nsw i64 %23, 4
  %159 = trunc i64 %132 to i32
  %160 = icmp eq i64 %158, 8
  br i1 %160, label %63, label %22, !llvm.loop !6
}

attributes #0 = { alwaysinline nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"double", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.unroll.disable"}
