; ModuleID = 'spmv.c'
source_filename = "spmv.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @ellpack(double* %0, i32* %1, double* %2, double* %3) #0 {
  %5 = alloca double*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca double*, align 8
  %8 = alloca double*, align 8
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca double, align 8
  %12 = alloca double, align 8
  store double* %0, double** %5, align 8
  store i32* %1, i32** %6, align 8
  store double* %2, double** %7, align 8
  store double* %3, double** %8, align 8
  br label %13

13:                                               ; preds = %4
  store i32 0, i32* %9, align 4
  br label %14

14:                                               ; preds = %61, %13
  %15 = load i32, i32* %9, align 4
  %16 = icmp slt i32 %15, 494
  br i1 %16, label %17, label %64

17:                                               ; preds = %14
  %18 = load double*, double** %8, align 8
  %19 = load i32, i32* %9, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds double, double* %18, i64 %20
  %22 = load double, double* %21, align 8
  store double %22, double* %12, align 8
  br label %23

23:                                               ; preds = %17
  store i32 0, i32* %10, align 4
  br label %24

24:                                               ; preds = %52, %23
  %25 = load i32, i32* %10, align 4
  %26 = icmp slt i32 %25, 10
  br i1 %26, label %27, label %55

27:                                               ; preds = %24
  %28 = load double*, double** %5, align 8
  %29 = load i32, i32* %10, align 4
  %30 = load i32, i32* %9, align 4
  %31 = mul nsw i32 %30, 10
  %32 = add nsw i32 %29, %31
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds double, double* %28, i64 %33
  %35 = load double, double* %34, align 8
  %36 = load double*, double** %7, align 8
  %37 = load i32*, i32** %6, align 8
  %38 = load i32, i32* %10, align 4
  %39 = load i32, i32* %9, align 4
  %40 = mul nsw i32 %39, 10
  %41 = add nsw i32 %38, %40
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds i32, i32* %37, i64 %42
  %44 = load i32, i32* %43, align 4
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds double, double* %36, i64 %45
  %47 = load double, double* %46, align 8
  %48 = fmul double %35, %47
  store double %48, double* %11, align 8
  %49 = load double, double* %11, align 8
  %50 = load double, double* %12, align 8
  %51 = fadd double %50, %49
  store double %51, double* %12, align 8
  br label %52

52:                                               ; preds = %27
  %53 = load i32, i32* %10, align 4
  %54 = add nsw i32 %53, 1
  store i32 %54, i32* %10, align 4
  br label %24

55:                                               ; preds = %24
  %56 = load double, double* %12, align 8
  %57 = load double*, double** %8, align 8
  %58 = load i32, i32* %9, align 4
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds double, double* %57, i64 %59
  store double %56, double* %60, align 8
  br label %61

61:                                               ; preds = %55
  %62 = load i32, i32* %9, align 4
  %63 = add nsw i32 %62, 1
  store i32 %63, i32* %9, align 4
  br label %14

64:                                               ; preds = %14
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
