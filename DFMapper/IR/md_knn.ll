; ModuleID = 'md.c'
source_filename = "md.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local i32 @md_kernel(double* nocapture %0, double* nocapture %1, double* nocapture %2, double* nocapture readonly %3, double* nocapture readonly %4, double* nocapture readonly %5, i32* nocapture readonly %6) local_unnamed_addr #0 {
  br label %8

8:                                                ; preds = %7, %65
  %9 = phi i64 [ 0, %7 ], [ %71, %65 ]
  %10 = phi i32 [ undef, %7 ], [ %62, %65 ]
  %11 = getelementptr inbounds double, double* %3, i64 %9
  %12 = load double, double* %11, align 8, !tbaa !2
  %13 = getelementptr inbounds double, double* %4, i64 %9
  %14 = load double, double* %13, align 8, !tbaa !2
  %15 = getelementptr inbounds double, double* %5, i64 %9
  %16 = load double, double* %15, align 8, !tbaa !2
  %17 = shl nsw i64 %9, 4
  %18 = insertelement <2 x double> undef, double %12, i32 0
  %19 = insertelement <2 x double> %18, double %16, i32 1
  br label %20

20:                                               ; preds = %8, %20
  %21 = phi i64 [ 0, %8 ], [ %63, %20 ]
  %22 = phi i32 [ %10, %8 ], [ %62, %20 ]
  %23 = phi double [ 0.000000e+00, %8 ], [ %57, %20 ]
  %24 = phi <2 x double> [ zeroinitializer, %8 ], [ %61, %20 ]
  %25 = sitofp i32 %22 to double
  %26 = extractelement <2 x double> %24, i32 0
  %27 = fadd double %26, %25
  %28 = fptosi double %27 to i32
  %29 = add nuw nsw i64 %21, %17
  %30 = getelementptr inbounds i32, i32* %6, i64 %29
  %31 = load i32, i32* %30, align 4, !tbaa !6
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds double, double* %3, i64 %32
  %34 = load double, double* %33, align 8, !tbaa !2
  %35 = getelementptr inbounds double, double* %4, i64 %32
  %36 = load double, double* %35, align 8, !tbaa !2
  %37 = getelementptr inbounds double, double* %5, i64 %32
  %38 = load double, double* %37, align 8, !tbaa !2
  %39 = fsub double %14, %36
  %40 = insertelement <2 x double> undef, double %34, i32 0
  %41 = insertelement <2 x double> %40, double %38, i32 1
  %42 = fsub <2 x double> %19, %41
  %43 = fmul double %39, %39
  %44 = fmul <2 x double> %42, %42
  %45 = extractelement <2 x double> %44, i32 0
  %46 = fadd double %45, %43
  %47 = extractelement <2 x double> %44, i32 1
  %48 = fadd double %46, %47
  %49 = fdiv double 1.000000e+00, %48
  %50 = fmul double %49, %49
  %51 = fmul double %49, %50
  %52 = fmul double %51, 1.500000e+00
  %53 = fadd double %52, -2.000000e+00
  %54 = fmul double %51, %53
  %55 = fmul double %49, %54
  %56 = fmul double %39, %55
  %57 = fadd double %23, %56
  %58 = insertelement <2 x double> undef, double %55, i32 0
  %59 = shufflevector <2 x double> %58, <2 x double> undef, <2 x i32> zeroinitializer
  %60 = fmul <2 x double> %42, %59
  %61 = fadd <2 x double> %24, %60
  %62 = add nsw i32 %28, 1024
  %63 = add nuw nsw i64 %21, 1
  %64 = icmp eq i64 %63, 16
  br i1 %64, label %65, label %20

65:                                               ; preds = %20
  %66 = getelementptr inbounds double, double* %0, i64 %9
  %67 = extractelement <2 x double> %61, i32 0
  store double %67, double* %66, align 8, !tbaa !2
  %68 = getelementptr inbounds double, double* %1, i64 %9
  store double %57, double* %68, align 8, !tbaa !2
  %69 = getelementptr inbounds double, double* %2, i64 %9
  %70 = extractelement <2 x double> %61, i32 1
  store double %70, double* %69, align 8, !tbaa !2
  %71 = add nuw nsw i64 %9, 1
  %72 = icmp eq i64 %71, 256
  br i1 %72, label %73, label %8

73:                                               ; preds = %65
  ret i32 %62
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
