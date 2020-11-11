; ModuleID = 'md.c'
source_filename = "md.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Out = type { i32 }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @md_kernel(double* %0, double* %1, double* %2, double* %3, double* %4, double* %5, i32* %6) #0 {
  %8 = alloca double*, align 8
  %9 = alloca double*, align 8
  %10 = alloca double*, align 8
  %11 = alloca double*, align 8
  %12 = alloca double*, align 8
  %13 = alloca double*, align 8
  %14 = alloca i32*, align 8
  %15 = alloca double, align 8
  %16 = alloca double, align 8
  %17 = alloca double, align 8
  %18 = alloca double, align 8
  %19 = alloca double, align 8
  %20 = alloca double, align 8
  %21 = alloca double, align 8
  %22 = alloca double, align 8
  %23 = alloca double, align 8
  %24 = alloca double, align 8
  %25 = alloca double, align 8
  %26 = alloca double, align 8
  %27 = alloca double, align 8
  %28 = alloca double, align 8
  %29 = alloca double, align 8
  %30 = alloca double, align 8
  %31 = alloca i32, align 4
  %32 = alloca i32, align 4
  %33 = alloca i32, align 4
  %34 = alloca %struct.Out, align 4
  store double* %0, double** %8, align 8
  store double* %1, double** %9, align 8
  store double* %2, double** %10, align 8
  store double* %3, double** %11, align 8
  store double* %4, double** %12, align 8
  store double* %5, double** %13, align 8
  store i32* %6, i32** %14, align 8
  br label %35

35:                                               ; preds = %7
  store i32 0, i32* %31, align 4
  br label %36

36:                                               ; preds = %160, %35
  %37 = load i32, i32* %31, align 4
  %38 = icmp slt i32 %37, 256
  br i1 %38, label %39, label %163

39:                                               ; preds = %36
  %40 = load double*, double** %11, align 8
  %41 = load i32, i32* %31, align 4
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds double, double* %40, i64 %42
  %44 = load double, double* %43, align 8
  store double %44, double* %25, align 8
  %45 = load double*, double** %12, align 8
  %46 = load i32, i32* %31, align 4
  %47 = sext i32 %46 to i64
  %48 = getelementptr inbounds double, double* %45, i64 %47
  %49 = load double, double* %48, align 8
  store double %49, double* %26, align 8
  %50 = load double*, double** %13, align 8
  %51 = load i32, i32* %31, align 4
  %52 = sext i32 %51 to i64
  %53 = getelementptr inbounds double, double* %50, i64 %52
  %54 = load double, double* %53, align 8
  store double %54, double* %27, align 8
  store double 0.000000e+00, double* %28, align 8
  store double 0.000000e+00, double* %29, align 8
  store double 0.000000e+00, double* %30, align 8
  br label %55

55:                                               ; preds = %39
  store i32 0, i32* %32, align 4
  br label %56

56:                                               ; preds = %141, %55
  %57 = load i32, i32* %32, align 4
  %58 = icmp slt i32 %57, 16
  br i1 %58, label %59, label %144

59:                                               ; preds = %56
  %60 = load double, double* %28, align 8
  %61 = getelementptr inbounds %struct.Out, %struct.Out* %34, i32 0, i32 0
  %62 = load i32, i32* %61, align 4
  %63 = sitofp i32 %62 to double
  %64 = fadd double %63, %60
  %65 = fptosi double %64 to i32
  store i32 %65, i32* %61, align 4
  %66 = load i32*, i32** %14, align 8
  %67 = load i32, i32* %31, align 4
  %68 = mul nsw i32 %67, 16
  %69 = load i32, i32* %32, align 4
  %70 = add nsw i32 %68, %69
  %71 = sext i32 %70 to i64
  %72 = getelementptr inbounds i32, i32* %66, i64 %71
  %73 = load i32, i32* %72, align 4
  store i32 %73, i32* %33, align 4
  %74 = load double*, double** %11, align 8
  %75 = load i32, i32* %33, align 4
  %76 = sext i32 %75 to i64
  %77 = getelementptr inbounds double, double* %74, i64 %76
  %78 = load double, double* %77, align 8
  store double %78, double* %22, align 8
  %79 = load double*, double** %12, align 8
  %80 = load i32, i32* %33, align 4
  %81 = sext i32 %80 to i64
  %82 = getelementptr inbounds double, double* %79, i64 %81
  %83 = load double, double* %82, align 8
  store double %83, double* %23, align 8
  %84 = load double*, double** %13, align 8
  %85 = load i32, i32* %33, align 4
  %86 = sext i32 %85 to i64
  %87 = getelementptr inbounds double, double* %84, i64 %86
  %88 = load double, double* %87, align 8
  store double %88, double* %24, align 8
  %89 = load double, double* %25, align 8
  %90 = load double, double* %22, align 8
  %91 = fsub double %89, %90
  store double %91, double* %15, align 8
  %92 = load double, double* %26, align 8
  %93 = load double, double* %23, align 8
  %94 = fsub double %92, %93
  store double %94, double* %16, align 8
  %95 = load double, double* %27, align 8
  %96 = load double, double* %24, align 8
  %97 = fsub double %95, %96
  store double %97, double* %17, align 8
  %98 = load double, double* %15, align 8
  %99 = load double, double* %15, align 8
  %100 = fmul double %98, %99
  %101 = load double, double* %16, align 8
  %102 = load double, double* %16, align 8
  %103 = fmul double %101, %102
  %104 = fadd double %100, %103
  %105 = load double, double* %17, align 8
  %106 = load double, double* %17, align 8
  %107 = fmul double %105, %106
  %108 = fadd double %104, %107
  %109 = fdiv double 1.000000e+00, %108
  store double %109, double* %18, align 8
  %110 = load double, double* %18, align 8
  %111 = load double, double* %18, align 8
  %112 = fmul double %110, %111
  %113 = load double, double* %18, align 8
  %114 = fmul double %112, %113
  store double %114, double* %19, align 8
  %115 = load double, double* %19, align 8
  %116 = load double, double* %19, align 8
  %117 = fmul double 1.500000e+00, %116
  %118 = fsub double %117, 2.000000e+00
  %119 = fmul double %115, %118
  store double %119, double* %20, align 8
  %120 = load double, double* %18, align 8
  %121 = load double, double* %20, align 8
  %122 = fmul double %120, %121
  store double %122, double* %21, align 8
  %123 = load double, double* %15, align 8
  %124 = load double, double* %21, align 8
  %125 = fmul double %123, %124
  %126 = load double, double* %28, align 8
  %127 = fadd double %126, %125
  store double %127, double* %28, align 8
  %128 = load double, double* %16, align 8
  %129 = load double, double* %21, align 8
  %130 = fmul double %128, %129
  %131 = load double, double* %29, align 8
  %132 = fadd double %131, %130
  store double %132, double* %29, align 8
  %133 = load double, double* %17, align 8
  %134 = load double, double* %21, align 8
  %135 = fmul double %133, %134
  %136 = load double, double* %30, align 8
  %137 = fadd double %136, %135
  store double %137, double* %30, align 8
  %138 = getelementptr inbounds %struct.Out, %struct.Out* %34, i32 0, i32 0
  %139 = load i32, i32* %138, align 4
  %140 = add nsw i32 %139, 1024
  store i32 %140, i32* %138, align 4
  br label %141

141:                                              ; preds = %59
  %142 = load i32, i32* %32, align 4
  %143 = add nsw i32 %142, 1
  store i32 %143, i32* %32, align 4
  br label %56

144:                                              ; preds = %56
  %145 = load double, double* %28, align 8
  %146 = load double*, double** %8, align 8
  %147 = load i32, i32* %31, align 4
  %148 = sext i32 %147 to i64
  %149 = getelementptr inbounds double, double* %146, i64 %148
  store double %145, double* %149, align 8
  %150 = load double, double* %29, align 8
  %151 = load double*, double** %9, align 8
  %152 = load i32, i32* %31, align 4
  %153 = sext i32 %152 to i64
  %154 = getelementptr inbounds double, double* %151, i64 %153
  store double %150, double* %154, align 8
  %155 = load double, double* %30, align 8
  %156 = load double*, double** %10, align 8
  %157 = load i32, i32* %31, align 4
  %158 = sext i32 %157 to i64
  %159 = getelementptr inbounds double, double* %156, i64 %158
  store double %155, double* %159, align 8
  br label %160

160:                                              ; preds = %144
  %161 = load i32, i32* %31, align 4
  %162 = add nsw i32 %161, 1
  store i32 %162, i32* %31, align 4
  br label %36

163:                                              ; preds = %36
  %164 = getelementptr inbounds %struct.Out, %struct.Out* %34, i32 0, i32 0
  %165 = load i32, i32* %164, align 4
  ret i32 %165
}

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
