; ModuleID = 'fft.c'
source_filename = "fft.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @fft(double* nocapture %0, double* nocapture %1, double* nocapture readonly %2, double* nocapture readonly %3) local_unnamed_addr #0 {
  br label %5

5:                                                ; preds = %4, %47
  %6 = phi i32 [ 0, %4 ], [ %49, %47 ]
  %7 = phi i32 [ 512, %4 ], [ %48, %47 ]
  %8 = icmp slt i32 %7, 1024
  br i1 %8, label %9, label %47

9:                                                ; preds = %5, %44
  %10 = phi i32 [ %45, %44 ], [ %7, %5 ]
  %11 = or i32 %10, %7
  %12 = xor i32 %11, %7
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds double, double* %0, i64 %13
  %15 = load double, double* %14, align 8, !tbaa !2
  %16 = sext i32 %11 to i64
  %17 = getelementptr inbounds double, double* %0, i64 %16
  %18 = load double, double* %17, align 8, !tbaa !2
  %19 = fadd double %15, %18
  %20 = fsub double %15, %18
  store double %20, double* %17, align 8, !tbaa !2
  store double %19, double* %14, align 8, !tbaa !2
  %21 = getelementptr inbounds double, double* %1, i64 %13
  %22 = load double, double* %21, align 8, !tbaa !2
  %23 = getelementptr inbounds double, double* %1, i64 %16
  %24 = load double, double* %23, align 8, !tbaa !2
  %25 = fadd double %22, %24
  %26 = fsub double %22, %24
  store double %26, double* %23, align 8, !tbaa !2
  store double %25, double* %21, align 8, !tbaa !2
  %27 = shl i32 %12, %6
  %28 = and i32 %27, 1023
  %29 = icmp eq i32 %28, 0
  br i1 %29, label %44, label %30

30:                                               ; preds = %9
  %31 = zext i32 %28 to i64
  %32 = getelementptr inbounds double, double* %2, i64 %31
  %33 = load double, double* %32, align 8, !tbaa !2
  %34 = load double, double* %17, align 8, !tbaa !2
  %35 = fmul double %33, %34
  %36 = getelementptr inbounds double, double* %3, i64 %31
  %37 = load double, double* %36, align 8, !tbaa !2
  %38 = load double, double* %23, align 8, !tbaa !2
  %39 = fmul double %37, %38
  %40 = fsub double %35, %39
  %41 = fmul double %33, %38
  %42 = fmul double %34, %37
  %43 = fadd double %42, %41
  store double %43, double* %23, align 8, !tbaa !2
  store double %40, double* %17, align 8, !tbaa !2
  br label %44

44:                                               ; preds = %9, %30
  %45 = add nsw i32 %11, 1
  %46 = icmp slt i32 %11, 1023
  br i1 %46, label %9, label %47

47:                                               ; preds = %44, %5
  %48 = ashr i32 %7, 1
  %49 = add nuw nsw i32 %6, 1
  %50 = icmp eq i32 %49, 10
  br i1 %50, label %51, label %5

51:                                               ; preds = %47
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
