; ModuleID = 'fft.c'
source_filename = "fft.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@__const.twiddles8.reversed8 = private unnamed_addr constant [8 x i32] [i32 0, i32 4, i32 2, i32 6, i32 1, i32 5, i32 3, i32 7], align 16

; Function Attrs: alwaysinline nofree nounwind optsize uwtable
define dso_local void @twiddles8(double* nocapture %0, double* nocapture %1, i32 %2, i32 %3) local_unnamed_addr #0 {
  %5 = sitofp i32 %3 to double
  %6 = sitofp i32 %2 to double
  br label %7

7:                                                ; preds = %4, %7
  %8 = phi i64 [ 1, %4 ], [ %28, %7 ]
  %9 = getelementptr inbounds [8 x i32], [8 x i32]* @__const.twiddles8.reversed8, i64 0, i64 %8
  %10 = load i32, i32* %9, align 4, !tbaa !2
  %11 = sitofp i32 %10 to double
  %12 = fmul double %11, 0xC01921FB54411744
  %13 = fdiv double %12, %5
  %14 = fmul double %13, %6
  %15 = tail call double @cos(double %14) #5
  %16 = tail call double @sin(double %14) #5
  %17 = getelementptr inbounds double, double* %0, i64 %8
  %18 = load double, double* %17, align 8, !tbaa !6
  %19 = fmul double %15, %18
  %20 = getelementptr inbounds double, double* %1, i64 %8
  %21 = load double, double* %20, align 8, !tbaa !6
  %22 = fmul double %16, %21
  %23 = fsub double %19, %22
  store double %23, double* %17, align 8, !tbaa !6
  %24 = fmul double %16, %18
  %25 = load double, double* %20, align 8, !tbaa !6
  %26 = fmul double %15, %25
  %27 = fadd double %24, %26
  store double %27, double* %20, align 8, !tbaa !6
  %28 = add nuw nsw i64 %8, 1
  %29 = icmp eq i64 %28, 8
  br i1 %29, label %30, label %7

30:                                               ; preds = %7
  ret void
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree nounwind optsize
declare dso_local double @cos(double) local_unnamed_addr #2

; Function Attrs: nofree nounwind optsize
declare dso_local double @sin(double) local_unnamed_addr #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @loadx8(double* nocapture %0, double* nocapture readonly %1, i32 %2, i32 %3) local_unnamed_addr #3 {
  %5 = sext i32 %2 to i64
  %6 = getelementptr inbounds double, double* %1, i64 %5
  %7 = load double, double* %6, align 8, !tbaa !6
  store double %7, double* %0, align 8, !tbaa !6
  %8 = add nsw i32 %3, %2
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds double, double* %1, i64 %9
  %11 = load double, double* %10, align 8, !tbaa !6
  %12 = getelementptr inbounds double, double* %0, i64 1
  store double %11, double* %12, align 8, !tbaa !6
  %13 = shl nsw i32 %3, 1
  %14 = add nsw i32 %13, %2
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds double, double* %1, i64 %15
  %17 = load double, double* %16, align 8, !tbaa !6
  %18 = getelementptr inbounds double, double* %0, i64 2
  store double %17, double* %18, align 8, !tbaa !6
  %19 = mul nsw i32 %3, 3
  %20 = add nsw i32 %19, %2
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds double, double* %1, i64 %21
  %23 = load double, double* %22, align 8, !tbaa !6
  %24 = getelementptr inbounds double, double* %0, i64 3
  store double %23, double* %24, align 8, !tbaa !6
  %25 = shl nsw i32 %3, 2
  %26 = add nsw i32 %25, %2
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds double, double* %1, i64 %27
  %29 = load double, double* %28, align 8, !tbaa !6
  %30 = getelementptr inbounds double, double* %0, i64 4
  store double %29, double* %30, align 8, !tbaa !6
  %31 = mul nsw i32 %3, 5
  %32 = add nsw i32 %31, %2
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds double, double* %1, i64 %33
  %35 = load double, double* %34, align 8, !tbaa !6
  %36 = getelementptr inbounds double, double* %0, i64 5
  store double %35, double* %36, align 8, !tbaa !6
  %37 = mul nsw i32 %3, 6
  %38 = add nsw i32 %37, %2
  %39 = sext i32 %38 to i64
  %40 = getelementptr inbounds double, double* %1, i64 %39
  %41 = load double, double* %40, align 8, !tbaa !6
  %42 = getelementptr inbounds double, double* %0, i64 6
  store double %41, double* %42, align 8, !tbaa !6
  %43 = mul nsw i32 %3, 7
  %44 = add nsw i32 %43, %2
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds double, double* %1, i64 %45
  %47 = load double, double* %46, align 8, !tbaa !6
  %48 = getelementptr inbounds double, double* %0, i64 7
  store double %47, double* %48, align 8, !tbaa !6
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @loady8(double* nocapture %0, double* nocapture readonly %1, i32 %2, i32 %3) local_unnamed_addr #3 {
  %5 = sext i32 %2 to i64
  %6 = getelementptr inbounds double, double* %1, i64 %5
  %7 = load double, double* %6, align 8, !tbaa !6
  store double %7, double* %0, align 8, !tbaa !6
  %8 = add nsw i32 %3, %2
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds double, double* %1, i64 %9
  %11 = load double, double* %10, align 8, !tbaa !6
  %12 = getelementptr inbounds double, double* %0, i64 1
  store double %11, double* %12, align 8, !tbaa !6
  %13 = shl nsw i32 %3, 1
  %14 = add nsw i32 %13, %2
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds double, double* %1, i64 %15
  %17 = load double, double* %16, align 8, !tbaa !6
  %18 = getelementptr inbounds double, double* %0, i64 2
  store double %17, double* %18, align 8, !tbaa !6
  %19 = mul nsw i32 %3, 3
  %20 = add nsw i32 %19, %2
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds double, double* %1, i64 %21
  %23 = load double, double* %22, align 8, !tbaa !6
  %24 = getelementptr inbounds double, double* %0, i64 3
  store double %23, double* %24, align 8, !tbaa !6
  %25 = shl nsw i32 %3, 2
  %26 = add nsw i32 %25, %2
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds double, double* %1, i64 %27
  %29 = load double, double* %28, align 8, !tbaa !6
  %30 = getelementptr inbounds double, double* %0, i64 4
  store double %29, double* %30, align 8, !tbaa !6
  %31 = mul nsw i32 %3, 5
  %32 = add nsw i32 %31, %2
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds double, double* %1, i64 %33
  %35 = load double, double* %34, align 8, !tbaa !6
  %36 = getelementptr inbounds double, double* %0, i64 5
  store double %35, double* %36, align 8, !tbaa !6
  %37 = mul nsw i32 %3, 6
  %38 = add nsw i32 %37, %2
  %39 = sext i32 %38 to i64
  %40 = getelementptr inbounds double, double* %1, i64 %39
  %41 = load double, double* %40, align 8, !tbaa !6
  %42 = getelementptr inbounds double, double* %0, i64 6
  store double %41, double* %42, align 8, !tbaa !6
  %43 = mul nsw i32 %3, 7
  %44 = add nsw i32 %43, %2
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds double, double* %1, i64 %45
  %47 = load double, double* %46, align 8, !tbaa !6
  %48 = getelementptr inbounds double, double* %0, i64 7
  store double %47, double* %48, align 8, !tbaa !6
  ret void
}

; Function Attrs: nounwind optsize uwtable
define dso_local void @fft1D_512(double* nocapture %0, double* nocapture %1) local_unnamed_addr #4 {
  %3 = alloca [512 x double], align 16
  %4 = alloca [512 x double], align 16
  %5 = alloca [8 x double], align 16
  %6 = alloca [8 x double], align 16
  %7 = alloca [576 x double], align 16
  %8 = bitcast [512 x double]* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4096, i8* nonnull %8) #6
  %9 = bitcast [512 x double]* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4096, i8* nonnull %9) #6
  %10 = bitcast [8 x double]* %5 to i8*
  call void @llvm.lifetime.start.p0i8(i64 64, i8* nonnull %10) #6
  %11 = bitcast [8 x double]* %6 to i8*
  call void @llvm.lifetime.start.p0i8(i64 64, i8* nonnull %11) #6
  %12 = bitcast [576 x double]* %7 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4608, i8* nonnull %12) #6
  %13 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 0
  %14 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 1
  %15 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 2
  %16 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 3
  %17 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 4
  %18 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 5
  %19 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 6
  %20 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 7
  %21 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 0
  %22 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 1
  %23 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 2
  %24 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 3
  %25 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 4
  %26 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 5
  %27 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 6
  %28 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 7
  %29 = bitcast [8 x double]* %5 to <2 x double>*
  %30 = bitcast double* %15 to <2 x double>*
  %31 = bitcast double* %17 to <2 x double>*
  %32 = bitcast double* %19 to <2 x double>*
  %33 = bitcast [8 x double]* %6 to <2 x double>*
  %34 = bitcast double* %23 to <2 x double>*
  %35 = bitcast double* %25 to <2 x double>*
  %36 = bitcast double* %27 to <2 x double>*
  br label %37

37:                                               ; preds = %2, %173
  %38 = phi i64 [ 0, %2 ], [ %202, %173 ]
  %39 = getelementptr inbounds double, double* %0, i64 %38
  %40 = load double, double* %39, align 8, !tbaa !6
  %41 = add nuw nsw i64 %38, 64
  %42 = getelementptr inbounds double, double* %0, i64 %41
  %43 = load double, double* %42, align 8, !tbaa !6
  %44 = add nuw nsw i64 %38, 128
  %45 = getelementptr inbounds double, double* %0, i64 %44
  %46 = load double, double* %45, align 8, !tbaa !6
  %47 = add nuw nsw i64 %38, 192
  %48 = getelementptr inbounds double, double* %0, i64 %47
  %49 = load double, double* %48, align 8, !tbaa !6
  %50 = add nuw nsw i64 %38, 256
  %51 = getelementptr inbounds double, double* %0, i64 %50
  %52 = load double, double* %51, align 8, !tbaa !6
  %53 = add nuw nsw i64 %38, 320
  %54 = getelementptr inbounds double, double* %0, i64 %53
  %55 = load double, double* %54, align 8, !tbaa !6
  %56 = add nuw nsw i64 %38, 384
  %57 = getelementptr inbounds double, double* %0, i64 %56
  %58 = load double, double* %57, align 8, !tbaa !6
  %59 = add nuw nsw i64 %38, 448
  %60 = getelementptr inbounds double, double* %0, i64 %59
  %61 = load double, double* %60, align 8, !tbaa !6
  %62 = getelementptr inbounds double, double* %1, i64 %38
  %63 = load double, double* %62, align 8, !tbaa !6
  %64 = getelementptr inbounds double, double* %1, i64 %41
  %65 = load double, double* %64, align 8, !tbaa !6
  %66 = getelementptr inbounds double, double* %1, i64 %44
  %67 = load double, double* %66, align 8, !tbaa !6
  %68 = getelementptr inbounds double, double* %1, i64 %47
  %69 = load double, double* %68, align 8, !tbaa !6
  %70 = getelementptr inbounds double, double* %1, i64 %50
  %71 = load double, double* %70, align 8, !tbaa !6
  %72 = getelementptr inbounds double, double* %1, i64 %53
  %73 = load double, double* %72, align 8, !tbaa !6
  %74 = getelementptr inbounds double, double* %1, i64 %56
  %75 = load double, double* %74, align 8, !tbaa !6
  %76 = getelementptr inbounds double, double* %1, i64 %59
  %77 = load double, double* %76, align 8, !tbaa !6
  %78 = fadd double %40, %52
  %79 = fadd double %63, %71
  %80 = fsub double %40, %52
  %81 = fsub double %63, %71
  %82 = fadd double %43, %55
  %83 = fadd double %65, %73
  %84 = fsub double %43, %55
  %85 = fsub double %65, %73
  %86 = fadd double %46, %58
  %87 = fadd double %67, %75
  %88 = fsub double %46, %58
  %89 = fsub double %67, %75
  %90 = fadd double %49, %61
  %91 = fadd double %69, %77
  %92 = fsub double %49, %61
  %93 = fsub double %69, %77
  %94 = fadd double %84, %85
  %95 = fmul double %94, 0x3FE6A09E667F3BCD
  %96 = fsub double %85, %84
  %97 = fmul double %96, 0x3FE6A09E667F3BCD
  %98 = fmul double %88, 0.000000e+00
  %99 = fadd double %98, %89
  %100 = fmul double %89, 0.000000e+00
  %101 = fsub double %100, %88
  %102 = fsub double %93, %92
  %103 = fmul double %102, 0x3FE6A09E667F3BCD
  %104 = fneg double %93
  %105 = fsub double %104, %92
  %106 = fmul double %105, 0x3FE6A09E667F3BCD
  %107 = fadd double %78, %86
  %108 = fadd double %79, %87
  %109 = fsub double %78, %86
  %110 = fsub double %79, %87
  %111 = fadd double %82, %90
  %112 = fadd double %83, %91
  %113 = fsub double %82, %90
  %114 = fsub double %83, %91
  %115 = fmul double %113, 0.000000e+00
  %116 = fadd double %115, %114
  %117 = fneg double %113
  %118 = fmul double %114, 0.000000e+00
  %119 = fsub double %117, %118
  %120 = fadd double %107, %111
  store double %120, double* %13, align 16, !tbaa !6
  %121 = fadd double %108, %112
  store double %121, double* %21, align 16, !tbaa !6
  %122 = fsub double %107, %111
  store double %122, double* %14, align 8, !tbaa !6
  %123 = fsub double %108, %112
  store double %123, double* %22, align 8, !tbaa !6
  %124 = fadd double %109, %116
  store double %124, double* %15, align 16, !tbaa !6
  %125 = fadd double %110, %119
  store double %125, double* %23, align 16, !tbaa !6
  %126 = fsub double %109, %116
  store double %126, double* %16, align 8, !tbaa !6
  %127 = fsub double %110, %119
  store double %127, double* %24, align 8, !tbaa !6
  %128 = fadd double %80, %99
  %129 = fadd double %81, %101
  %130 = fsub double %80, %99
  %131 = fsub double %81, %101
  %132 = fadd double %95, %103
  %133 = fadd double %97, %106
  %134 = fsub double %95, %103
  %135 = fsub double %97, %106
  %136 = fmul double %134, 0.000000e+00
  %137 = fadd double %135, %136
  %138 = fneg double %134
  %139 = fmul double %135, 0.000000e+00
  %140 = fsub double %138, %139
  %141 = fadd double %128, %132
  store double %141, double* %17, align 16, !tbaa !6
  %142 = fadd double %129, %133
  store double %142, double* %25, align 16, !tbaa !6
  %143 = fsub double %128, %132
  store double %143, double* %18, align 8, !tbaa !6
  %144 = fsub double %129, %133
  store double %144, double* %26, align 8, !tbaa !6
  %145 = fadd double %130, %137
  store double %145, double* %19, align 16, !tbaa !6
  %146 = fadd double %131, %140
  store double %146, double* %27, align 16, !tbaa !6
  %147 = fsub double %130, %137
  store double %147, double* %20, align 8, !tbaa !6
  %148 = fsub double %131, %140
  store double %148, double* %28, align 8, !tbaa !6
  %149 = trunc i64 %38 to i32
  %150 = sitofp i32 %149 to double
  br label %151

151:                                              ; preds = %151, %37
  %152 = phi i64 [ 1, %37 ], [ %171, %151 ]
  %153 = getelementptr inbounds [8 x i32], [8 x i32]* @__const.twiddles8.reversed8, i64 0, i64 %152
  %154 = load i32, i32* %153, align 4, !tbaa !2
  %155 = sitofp i32 %154 to double
  %156 = fmul double %155, 0xC01921FB54411744
  %157 = fmul double %156, 0x3F60000000000000
  %158 = fmul double %157, %150
  %159 = tail call double @cos(double %158) #5
  %160 = tail call double @sin(double %158) #5
  %161 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 %152
  %162 = load double, double* %161, align 8, !tbaa !6
  %163 = fmul double %159, %162
  %164 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 %152
  %165 = load double, double* %164, align 8, !tbaa !6
  %166 = fmul double %160, %165
  %167 = fsub double %163, %166
  store double %167, double* %161, align 8, !tbaa !6
  %168 = fmul double %160, %162
  %169 = fmul double %159, %165
  %170 = fadd double %168, %169
  store double %170, double* %164, align 8, !tbaa !6
  %171 = add nuw nsw i64 %152, 1
  %172 = icmp eq i64 %171, 8
  br i1 %172, label %173, label %151

173:                                              ; preds = %151
  %174 = shl nsw i64 %38, 3
  %175 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %174
  %176 = load <2 x double>, <2 x double>* %29, align 16, !tbaa !6
  %177 = bitcast double* %175 to <2 x double>*
  store <2 x double> %176, <2 x double>* %177, align 16, !tbaa !6
  %178 = or i64 %174, 2
  %179 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %178
  %180 = load <2 x double>, <2 x double>* %30, align 16, !tbaa !6
  %181 = bitcast double* %179 to <2 x double>*
  store <2 x double> %180, <2 x double>* %181, align 16, !tbaa !6
  %182 = or i64 %174, 4
  %183 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %182
  %184 = load <2 x double>, <2 x double>* %31, align 16, !tbaa !6
  %185 = bitcast double* %183 to <2 x double>*
  store <2 x double> %184, <2 x double>* %185, align 16, !tbaa !6
  %186 = or i64 %174, 6
  %187 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %186
  %188 = load <2 x double>, <2 x double>* %32, align 16, !tbaa !6
  %189 = bitcast double* %187 to <2 x double>*
  store <2 x double> %188, <2 x double>* %189, align 16, !tbaa !6
  %190 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %174
  %191 = load <2 x double>, <2 x double>* %33, align 16, !tbaa !6
  %192 = bitcast double* %190 to <2 x double>*
  store <2 x double> %191, <2 x double>* %192, align 16, !tbaa !6
  %193 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %178
  %194 = load <2 x double>, <2 x double>* %34, align 16, !tbaa !6
  %195 = bitcast double* %193 to <2 x double>*
  store <2 x double> %194, <2 x double>* %195, align 16, !tbaa !6
  %196 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %182
  %197 = load <2 x double>, <2 x double>* %35, align 16, !tbaa !6
  %198 = bitcast double* %196 to <2 x double>*
  store <2 x double> %197, <2 x double>* %198, align 16, !tbaa !6
  %199 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %186
  %200 = load <2 x double>, <2 x double>* %36, align 16, !tbaa !6
  %201 = bitcast double* %199 to <2 x double>*
  store <2 x double> %200, <2 x double>* %201, align 16, !tbaa !6
  %202 = add nuw nsw i64 %38, 1
  %203 = icmp eq i64 %202, 64
  br i1 %203, label %204, label %37

204:                                              ; preds = %173, %204
  %205 = phi i64 [ %241, %204 ], [ 0, %173 ]
  %206 = shl nuw nsw i64 %205, 3
  %207 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %206
  %208 = bitcast double* %207 to <16 x double>*
  %209 = load <16 x double>, <16 x double>* %208, align 16, !tbaa !6
  %210 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 0, i32 8>
  %211 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 1, i32 9>
  %212 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 2, i32 10>
  %213 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 3, i32 11>
  %214 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 4, i32 12>
  %215 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 5, i32 13>
  %216 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 6, i32 14>
  %217 = shufflevector <16 x double> %209, <16 x double> undef, <2 x i32> <i32 7, i32 15>
  %218 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %205
  %219 = bitcast double* %218 to <2 x double>*
  store <2 x double> %210, <2 x double>* %219, align 16, !tbaa !6
  %220 = add nuw nsw i64 %205, 264
  %221 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %220
  %222 = bitcast double* %221 to <2 x double>*
  store <2 x double> %211, <2 x double>* %222, align 16, !tbaa !6
  %223 = add nuw nsw i64 %205, 66
  %224 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %223
  %225 = bitcast double* %224 to <2 x double>*
  store <2 x double> %214, <2 x double>* %225, align 16, !tbaa !6
  %226 = add nuw nsw i64 %205, 330
  %227 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %226
  %228 = bitcast double* %227 to <2 x double>*
  store <2 x double> %215, <2 x double>* %228, align 16, !tbaa !6
  %229 = add nuw nsw i64 %205, 132
  %230 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %229
  %231 = bitcast double* %230 to <2 x double>*
  store <2 x double> %212, <2 x double>* %231, align 16, !tbaa !6
  %232 = add nuw nsw i64 %205, 396
  %233 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %232
  %234 = bitcast double* %233 to <2 x double>*
  store <2 x double> %213, <2 x double>* %234, align 16, !tbaa !6
  %235 = add nuw nsw i64 %205, 198
  %236 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %235
  %237 = bitcast double* %236 to <2 x double>*
  store <2 x double> %216, <2 x double>* %237, align 16, !tbaa !6
  %238 = add nuw nsw i64 %205, 462
  %239 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %238
  %240 = bitcast double* %239 to <2 x double>*
  store <2 x double> %217, <2 x double>* %240, align 16, !tbaa !6
  %241 = add i64 %205, 2
  %242 = icmp eq i64 %241, 64
  br i1 %242, label %243, label %204, !llvm.loop !8

243:                                              ; preds = %204, %243
  %244 = phi i64 [ %297, %243 ], [ 0, %204 ]
  %245 = trunc i64 %244 to i32
  %246 = lshr i32 %245, 3
  %247 = and i32 %245, 7
  %248 = mul nuw nsw i32 %247, 66
  %249 = add nuw nsw i32 %248, %246
  %250 = zext i32 %249 to i64
  %251 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %250
  %252 = load double, double* %251, align 8, !tbaa !6
  %253 = shl nuw nsw i64 %244, 3
  %254 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %253
  store double %252, double* %254, align 16, !tbaa !6
  %255 = add nuw nsw i32 %249, 32
  %256 = zext i32 %255 to i64
  %257 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %256
  %258 = load double, double* %257, align 8, !tbaa !6
  %259 = or i64 %253, 4
  %260 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %259
  store double %258, double* %260, align 16, !tbaa !6
  %261 = add nuw nsw i32 %249, 8
  %262 = zext i32 %261 to i64
  %263 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %262
  %264 = load double, double* %263, align 8, !tbaa !6
  %265 = or i64 %253, 1
  %266 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %265
  store double %264, double* %266, align 8, !tbaa !6
  %267 = add nuw nsw i32 %249, 40
  %268 = zext i32 %267 to i64
  %269 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %268
  %270 = load double, double* %269, align 8, !tbaa !6
  %271 = or i64 %253, 5
  %272 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %271
  store double %270, double* %272, align 8, !tbaa !6
  %273 = add nuw nsw i32 %249, 16
  %274 = zext i32 %273 to i64
  %275 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %274
  %276 = load double, double* %275, align 8, !tbaa !6
  %277 = or i64 %253, 2
  %278 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %277
  store double %276, double* %278, align 16, !tbaa !6
  %279 = add nuw nsw i32 %249, 48
  %280 = zext i32 %279 to i64
  %281 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %280
  %282 = load double, double* %281, align 8, !tbaa !6
  %283 = or i64 %253, 6
  %284 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %283
  store double %282, double* %284, align 16, !tbaa !6
  %285 = add nuw nsw i32 %249, 24
  %286 = zext i32 %285 to i64
  %287 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %286
  %288 = load double, double* %287, align 8, !tbaa !6
  %289 = or i64 %253, 3
  %290 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %289
  store double %288, double* %290, align 8, !tbaa !6
  %291 = add nuw nsw i32 %249, 56
  %292 = zext i32 %291 to i64
  %293 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %292
  %294 = load double, double* %293, align 8, !tbaa !6
  %295 = or i64 %253, 7
  %296 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %295
  store double %294, double* %296, align 8, !tbaa !6
  %297 = add nuw nsw i64 %244, 1
  %298 = icmp eq i64 %297, 64
  br i1 %298, label %299, label %243

299:                                              ; preds = %243, %299
  %300 = phi i64 [ %336, %299 ], [ 0, %243 ]
  %301 = shl nuw nsw i64 %300, 3
  %302 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %301
  %303 = bitcast double* %302 to <16 x double>*
  %304 = load <16 x double>, <16 x double>* %303, align 16, !tbaa !6
  %305 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 0, i32 8>
  %306 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 1, i32 9>
  %307 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 2, i32 10>
  %308 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 3, i32 11>
  %309 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 4, i32 12>
  %310 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 5, i32 13>
  %311 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 6, i32 14>
  %312 = shufflevector <16 x double> %304, <16 x double> undef, <2 x i32> <i32 7, i32 15>
  %313 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %300
  %314 = bitcast double* %313 to <2 x double>*
  store <2 x double> %305, <2 x double>* %314, align 16, !tbaa !6
  %315 = add nuw nsw i64 %300, 264
  %316 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %315
  %317 = bitcast double* %316 to <2 x double>*
  store <2 x double> %306, <2 x double>* %317, align 16, !tbaa !6
  %318 = add nuw nsw i64 %300, 66
  %319 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %318
  %320 = bitcast double* %319 to <2 x double>*
  store <2 x double> %309, <2 x double>* %320, align 16, !tbaa !6
  %321 = add nuw nsw i64 %300, 330
  %322 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %321
  %323 = bitcast double* %322 to <2 x double>*
  store <2 x double> %310, <2 x double>* %323, align 16, !tbaa !6
  %324 = add nuw nsw i64 %300, 132
  %325 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %324
  %326 = bitcast double* %325 to <2 x double>*
  store <2 x double> %307, <2 x double>* %326, align 16, !tbaa !6
  %327 = add nuw nsw i64 %300, 396
  %328 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %327
  %329 = bitcast double* %328 to <2 x double>*
  store <2 x double> %308, <2 x double>* %329, align 16, !tbaa !6
  %330 = add nuw nsw i64 %300, 198
  %331 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %330
  %332 = bitcast double* %331 to <2 x double>*
  store <2 x double> %311, <2 x double>* %332, align 16, !tbaa !6
  %333 = add nuw nsw i64 %300, 462
  %334 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %333
  %335 = bitcast double* %334 to <2 x double>*
  store <2 x double> %312, <2 x double>* %335, align 16, !tbaa !6
  %336 = add i64 %300, 2
  %337 = icmp eq i64 %336, 64
  br i1 %337, label %347, label %299, !llvm.loop !10

338:                                              ; preds = %347
  store double %372, double* %21, align 16, !tbaa !6
  store double %376, double* %22, align 8, !tbaa !6
  store double %380, double* %23, align 16, !tbaa !6
  store double %384, double* %24, align 8, !tbaa !6
  store double %388, double* %25, align 16, !tbaa !6
  store double %392, double* %26, align 8, !tbaa !6
  store double %396, double* %27, align 16, !tbaa !6
  store double %400, double* %28, align 8, !tbaa !6
  %339 = bitcast [8 x double]* %5 to <2 x double>*
  %340 = bitcast double* %15 to <2 x double>*
  %341 = bitcast double* %17 to <2 x double>*
  %342 = bitcast double* %19 to <2 x double>*
  %343 = bitcast [8 x double]* %6 to <2 x double>*
  %344 = bitcast double* %23 to <2 x double>*
  %345 = bitcast double* %25 to <2 x double>*
  %346 = bitcast double* %27 to <2 x double>*
  br label %403

347:                                              ; preds = %299, %347
  %348 = phi i64 [ %401, %347 ], [ 0, %299 ]
  %349 = shl nuw nsw i64 %348, 3
  %350 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %349
  %351 = or i64 %349, 1
  %352 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %351
  %353 = or i64 %349, 2
  %354 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %353
  %355 = or i64 %349, 3
  %356 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %355
  %357 = or i64 %349, 4
  %358 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %357
  %359 = or i64 %349, 5
  %360 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %359
  %361 = or i64 %349, 6
  %362 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %361
  %363 = or i64 %349, 7
  %364 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %363
  %365 = trunc i64 %348 to i32
  %366 = lshr i32 %365, 3
  %367 = and i32 %365, 7
  %368 = mul nuw nsw i32 %367, 66
  %369 = add nuw nsw i32 %368, %366
  %370 = zext i32 %369 to i64
  %371 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %370
  %372 = load double, double* %371, align 8, !tbaa !6
  %373 = add nuw nsw i32 %369, 8
  %374 = zext i32 %373 to i64
  %375 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %374
  %376 = load double, double* %375, align 8, !tbaa !6
  %377 = add nuw nsw i32 %369, 16
  %378 = zext i32 %377 to i64
  %379 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %378
  %380 = load double, double* %379, align 8, !tbaa !6
  %381 = add nuw nsw i32 %369, 24
  %382 = zext i32 %381 to i64
  %383 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %382
  %384 = load double, double* %383, align 8, !tbaa !6
  %385 = add nuw nsw i32 %369, 32
  %386 = zext i32 %385 to i64
  %387 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %386
  %388 = load double, double* %387, align 8, !tbaa !6
  %389 = add nuw nsw i32 %369, 40
  %390 = zext i32 %389 to i64
  %391 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %390
  %392 = load double, double* %391, align 8, !tbaa !6
  %393 = add nuw nsw i32 %369, 48
  %394 = zext i32 %393 to i64
  %395 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %394
  %396 = load double, double* %395, align 8, !tbaa !6
  %397 = add nuw nsw i32 %369, 56
  %398 = zext i32 %397 to i64
  %399 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %398
  %400 = load double, double* %399, align 8, !tbaa !6
  store double %372, double* %350, align 16, !tbaa !6
  store double %376, double* %352, align 8, !tbaa !6
  store double %380, double* %354, align 16, !tbaa !6
  store double %384, double* %356, align 8, !tbaa !6
  store double %388, double* %358, align 16, !tbaa !6
  store double %392, double* %360, align 8, !tbaa !6
  store double %396, double* %362, align 16, !tbaa !6
  store double %400, double* %364, align 8, !tbaa !6
  %401 = add nuw nsw i64 %348, 1
  %402 = icmp eq i64 %401, 64
  br i1 %402, label %338, label %347

403:                                              ; preds = %338, %541
  %404 = phi i64 [ 0, %338 ], [ %558, %541 ]
  %405 = shl nuw nsw i64 %404, 3
  %406 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %405
  %407 = load double, double* %406, align 16, !tbaa !6
  %408 = or i64 %405, 1
  %409 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %408
  %410 = load double, double* %409, align 8, !tbaa !6
  %411 = or i64 %405, 2
  %412 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %411
  %413 = load double, double* %412, align 16, !tbaa !6
  %414 = or i64 %405, 3
  %415 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %414
  %416 = load double, double* %415, align 8, !tbaa !6
  %417 = or i64 %405, 4
  %418 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %417
  %419 = load double, double* %418, align 16, !tbaa !6
  %420 = or i64 %405, 5
  %421 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %420
  %422 = load double, double* %421, align 8, !tbaa !6
  %423 = or i64 %405, 6
  %424 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %423
  %425 = load double, double* %424, align 16, !tbaa !6
  %426 = or i64 %405, 7
  %427 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %426
  %428 = load double, double* %427, align 8, !tbaa !6
  %429 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %405
  %430 = load double, double* %429, align 16, !tbaa !6
  %431 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %408
  %432 = load double, double* %431, align 8, !tbaa !6
  %433 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %411
  %434 = load double, double* %433, align 16, !tbaa !6
  %435 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %414
  %436 = load double, double* %435, align 8, !tbaa !6
  %437 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %417
  %438 = load double, double* %437, align 16, !tbaa !6
  %439 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %420
  %440 = load double, double* %439, align 8, !tbaa !6
  %441 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %423
  %442 = load double, double* %441, align 16, !tbaa !6
  %443 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %426
  %444 = load double, double* %443, align 8, !tbaa !6
  %445 = fadd double %407, %419
  %446 = fadd double %430, %438
  %447 = fsub double %407, %419
  %448 = fsub double %430, %438
  %449 = fadd double %410, %422
  %450 = fadd double %432, %440
  %451 = fsub double %410, %422
  %452 = fsub double %432, %440
  %453 = fadd double %413, %425
  %454 = fadd double %434, %442
  %455 = fsub double %413, %425
  %456 = fsub double %434, %442
  %457 = fadd double %416, %428
  %458 = fadd double %436, %444
  %459 = fsub double %416, %428
  %460 = fsub double %436, %444
  %461 = fadd double %451, %452
  %462 = fmul double %461, 0x3FE6A09E667F3BCD
  %463 = fsub double %452, %451
  %464 = fmul double %463, 0x3FE6A09E667F3BCD
  %465 = fmul double %455, 0.000000e+00
  %466 = fadd double %465, %456
  %467 = fmul double %456, 0.000000e+00
  %468 = fsub double %467, %455
  %469 = fsub double %460, %459
  %470 = fmul double %469, 0x3FE6A09E667F3BCD
  %471 = fneg double %460
  %472 = fsub double %471, %459
  %473 = fmul double %472, 0x3FE6A09E667F3BCD
  %474 = fadd double %445, %453
  %475 = fadd double %446, %454
  %476 = fsub double %445, %453
  %477 = fsub double %446, %454
  %478 = fadd double %449, %457
  %479 = fadd double %450, %458
  %480 = fsub double %449, %457
  %481 = fsub double %450, %458
  %482 = fmul double %480, 0.000000e+00
  %483 = fadd double %482, %481
  %484 = fneg double %480
  %485 = fmul double %481, 0.000000e+00
  %486 = fsub double %484, %485
  %487 = fadd double %474, %478
  store double %487, double* %13, align 16, !tbaa !6
  %488 = fadd double %475, %479
  store double %488, double* %21, align 16, !tbaa !6
  %489 = fsub double %474, %478
  store double %489, double* %14, align 8, !tbaa !6
  %490 = fsub double %475, %479
  store double %490, double* %22, align 8, !tbaa !6
  %491 = fadd double %476, %483
  store double %491, double* %15, align 16, !tbaa !6
  %492 = fadd double %477, %486
  store double %492, double* %23, align 16, !tbaa !6
  %493 = fsub double %476, %483
  store double %493, double* %16, align 8, !tbaa !6
  %494 = fsub double %477, %486
  store double %494, double* %24, align 8, !tbaa !6
  %495 = fadd double %447, %466
  %496 = fadd double %448, %468
  %497 = fsub double %447, %466
  %498 = fsub double %448, %468
  %499 = fadd double %462, %470
  %500 = fadd double %464, %473
  %501 = fsub double %462, %470
  %502 = fsub double %464, %473
  %503 = fmul double %501, 0.000000e+00
  %504 = fadd double %502, %503
  %505 = fneg double %501
  %506 = fmul double %502, 0.000000e+00
  %507 = fsub double %505, %506
  %508 = fadd double %495, %499
  store double %508, double* %17, align 16, !tbaa !6
  %509 = fadd double %496, %500
  store double %509, double* %25, align 16, !tbaa !6
  %510 = fsub double %495, %499
  store double %510, double* %18, align 8, !tbaa !6
  %511 = fsub double %496, %500
  store double %511, double* %26, align 8, !tbaa !6
  %512 = fadd double %497, %504
  store double %512, double* %19, align 16, !tbaa !6
  %513 = fadd double %498, %507
  store double %513, double* %27, align 16, !tbaa !6
  %514 = fsub double %497, %504
  store double %514, double* %20, align 8, !tbaa !6
  %515 = fsub double %498, %507
  store double %515, double* %28, align 8, !tbaa !6
  %516 = trunc i64 %404 to i32
  %517 = lshr i32 %516, 3
  %518 = sitofp i32 %517 to double
  br label %519

519:                                              ; preds = %519, %403
  %520 = phi i64 [ 1, %403 ], [ %539, %519 ]
  %521 = getelementptr inbounds [8 x i32], [8 x i32]* @__const.twiddles8.reversed8, i64 0, i64 %520
  %522 = load i32, i32* %521, align 4, !tbaa !2
  %523 = sitofp i32 %522 to double
  %524 = fmul double %523, 0xC01921FB54411744
  %525 = fmul double %524, 1.562500e-02
  %526 = fmul double %525, %518
  %527 = tail call double @cos(double %526) #5
  %528 = tail call double @sin(double %526) #5
  %529 = getelementptr inbounds [8 x double], [8 x double]* %5, i64 0, i64 %520
  %530 = load double, double* %529, align 8, !tbaa !6
  %531 = fmul double %527, %530
  %532 = getelementptr inbounds [8 x double], [8 x double]* %6, i64 0, i64 %520
  %533 = load double, double* %532, align 8, !tbaa !6
  %534 = fmul double %528, %533
  %535 = fsub double %531, %534
  store double %535, double* %529, align 8, !tbaa !6
  %536 = fmul double %528, %530
  %537 = fmul double %527, %533
  %538 = fadd double %536, %537
  store double %538, double* %532, align 8, !tbaa !6
  %539 = add nuw nsw i64 %520, 1
  %540 = icmp eq i64 %539, 8
  br i1 %540, label %541, label %519

541:                                              ; preds = %519
  %542 = load <2 x double>, <2 x double>* %339, align 16, !tbaa !6
  %543 = bitcast double* %406 to <2 x double>*
  store <2 x double> %542, <2 x double>* %543, align 16, !tbaa !6
  %544 = load <2 x double>, <2 x double>* %340, align 16, !tbaa !6
  %545 = bitcast double* %412 to <2 x double>*
  store <2 x double> %544, <2 x double>* %545, align 16, !tbaa !6
  %546 = load <2 x double>, <2 x double>* %341, align 16, !tbaa !6
  %547 = bitcast double* %418 to <2 x double>*
  store <2 x double> %546, <2 x double>* %547, align 16, !tbaa !6
  %548 = load <2 x double>, <2 x double>* %342, align 16, !tbaa !6
  %549 = bitcast double* %424 to <2 x double>*
  store <2 x double> %548, <2 x double>* %549, align 16, !tbaa !6
  %550 = load <2 x double>, <2 x double>* %343, align 16, !tbaa !6
  %551 = bitcast double* %429 to <2 x double>*
  store <2 x double> %550, <2 x double>* %551, align 16, !tbaa !6
  %552 = load <2 x double>, <2 x double>* %344, align 16, !tbaa !6
  %553 = bitcast double* %433 to <2 x double>*
  store <2 x double> %552, <2 x double>* %553, align 16, !tbaa !6
  %554 = load <2 x double>, <2 x double>* %345, align 16, !tbaa !6
  %555 = bitcast double* %437 to <2 x double>*
  store <2 x double> %554, <2 x double>* %555, align 16, !tbaa !6
  %556 = load <2 x double>, <2 x double>* %346, align 16, !tbaa !6
  %557 = bitcast double* %441 to <2 x double>*
  store <2 x double> %556, <2 x double>* %557, align 16, !tbaa !6
  %558 = add nuw nsw i64 %404, 1
  %559 = icmp eq i64 %558, 64
  br i1 %559, label %560, label %403

560:                                              ; preds = %541, %560
  %561 = phi i64 [ %597, %560 ], [ 0, %541 ]
  %562 = shl nuw nsw i64 %561, 3
  %563 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %562
  %564 = bitcast double* %563 to <16 x double>*
  %565 = load <16 x double>, <16 x double>* %564, align 16, !tbaa !6
  %566 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 0, i32 8>
  %567 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 1, i32 9>
  %568 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 2, i32 10>
  %569 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 3, i32 11>
  %570 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 4, i32 12>
  %571 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 5, i32 13>
  %572 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 6, i32 14>
  %573 = shufflevector <16 x double> %565, <16 x double> undef, <2 x i32> <i32 7, i32 15>
  %574 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %561
  %575 = bitcast double* %574 to <2 x double>*
  store <2 x double> %566, <2 x double>* %575, align 16, !tbaa !6
  %576 = add nuw nsw i64 %561, 288
  %577 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %576
  %578 = bitcast double* %577 to <2 x double>*
  store <2 x double> %567, <2 x double>* %578, align 16, !tbaa !6
  %579 = add nuw nsw i64 %561, 72
  %580 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %579
  %581 = bitcast double* %580 to <2 x double>*
  store <2 x double> %570, <2 x double>* %581, align 16, !tbaa !6
  %582 = add nuw nsw i64 %561, 360
  %583 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %582
  %584 = bitcast double* %583 to <2 x double>*
  store <2 x double> %571, <2 x double>* %584, align 16, !tbaa !6
  %585 = add nuw nsw i64 %561, 144
  %586 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %585
  %587 = bitcast double* %586 to <2 x double>*
  store <2 x double> %568, <2 x double>* %587, align 16, !tbaa !6
  %588 = add nuw nsw i64 %561, 432
  %589 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %588
  %590 = bitcast double* %589 to <2 x double>*
  store <2 x double> %569, <2 x double>* %590, align 16, !tbaa !6
  %591 = add nuw nsw i64 %561, 216
  %592 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %591
  %593 = bitcast double* %592 to <2 x double>*
  store <2 x double> %572, <2 x double>* %593, align 16, !tbaa !6
  %594 = add nuw nsw i64 %561, 504
  %595 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %594
  %596 = bitcast double* %595 to <2 x double>*
  store <2 x double> %573, <2 x double>* %596, align 16, !tbaa !6
  %597 = add i64 %561, 2
  %598 = icmp eq i64 %597, 64
  br i1 %598, label %599, label %560, !llvm.loop !11

599:                                              ; preds = %560, %599
  %600 = phi i64 [ %653, %599 ], [ 0, %560 ]
  %601 = trunc i64 %600 to i32
  %602 = lshr i32 %601, 3
  %603 = and i32 %601, 7
  %604 = mul nuw nsw i32 %602, 72
  %605 = or i32 %604, %603
  %606 = zext i32 %605 to i64
  %607 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %606
  %608 = load double, double* %607, align 8, !tbaa !6
  %609 = shl nuw nsw i64 %600, 3
  %610 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %609
  store double %608, double* %610, align 16, !tbaa !6
  %611 = add nuw nsw i32 %605, 32
  %612 = zext i32 %611 to i64
  %613 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %612
  %614 = load double, double* %613, align 8, !tbaa !6
  %615 = or i64 %609, 4
  %616 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %615
  store double %614, double* %616, align 16, !tbaa !6
  %617 = add nuw nsw i32 %605, 8
  %618 = zext i32 %617 to i64
  %619 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %618
  %620 = load double, double* %619, align 8, !tbaa !6
  %621 = or i64 %609, 1
  %622 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %621
  store double %620, double* %622, align 8, !tbaa !6
  %623 = add nuw nsw i32 %605, 40
  %624 = zext i32 %623 to i64
  %625 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %624
  %626 = load double, double* %625, align 8, !tbaa !6
  %627 = or i64 %609, 5
  %628 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %627
  store double %626, double* %628, align 8, !tbaa !6
  %629 = add nuw nsw i32 %605, 16
  %630 = zext i32 %629 to i64
  %631 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %630
  %632 = load double, double* %631, align 8, !tbaa !6
  %633 = or i64 %609, 2
  %634 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %633
  store double %632, double* %634, align 16, !tbaa !6
  %635 = add nuw nsw i32 %605, 48
  %636 = zext i32 %635 to i64
  %637 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %636
  %638 = load double, double* %637, align 8, !tbaa !6
  %639 = or i64 %609, 6
  %640 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %639
  store double %638, double* %640, align 16, !tbaa !6
  %641 = add nuw nsw i32 %605, 24
  %642 = zext i32 %641 to i64
  %643 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %642
  %644 = load double, double* %643, align 8, !tbaa !6
  %645 = or i64 %609, 3
  %646 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %645
  store double %644, double* %646, align 8, !tbaa !6
  %647 = add nuw nsw i32 %605, 56
  %648 = zext i32 %647 to i64
  %649 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %648
  %650 = load double, double* %649, align 8, !tbaa !6
  %651 = or i64 %609, 7
  %652 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %651
  store double %650, double* %652, align 8, !tbaa !6
  %653 = add nuw nsw i64 %600, 1
  %654 = icmp eq i64 %653, 64
  br i1 %654, label %655, label %599

655:                                              ; preds = %599, %655
  %656 = phi i64 [ %692, %655 ], [ 0, %599 ]
  %657 = shl nuw nsw i64 %656, 3
  %658 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %657
  %659 = bitcast double* %658 to <16 x double>*
  %660 = load <16 x double>, <16 x double>* %659, align 16, !tbaa !6
  %661 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 0, i32 8>
  %662 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 1, i32 9>
  %663 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 2, i32 10>
  %664 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 3, i32 11>
  %665 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 4, i32 12>
  %666 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 5, i32 13>
  %667 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 6, i32 14>
  %668 = shufflevector <16 x double> %660, <16 x double> undef, <2 x i32> <i32 7, i32 15>
  %669 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %656
  %670 = bitcast double* %669 to <2 x double>*
  store <2 x double> %661, <2 x double>* %670, align 16, !tbaa !6
  %671 = add nuw nsw i64 %656, 288
  %672 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %671
  %673 = bitcast double* %672 to <2 x double>*
  store <2 x double> %662, <2 x double>* %673, align 16, !tbaa !6
  %674 = add nuw nsw i64 %656, 72
  %675 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %674
  %676 = bitcast double* %675 to <2 x double>*
  store <2 x double> %665, <2 x double>* %676, align 16, !tbaa !6
  %677 = add nuw nsw i64 %656, 360
  %678 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %677
  %679 = bitcast double* %678 to <2 x double>*
  store <2 x double> %666, <2 x double>* %679, align 16, !tbaa !6
  %680 = add nuw nsw i64 %656, 144
  %681 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %680
  %682 = bitcast double* %681 to <2 x double>*
  store <2 x double> %663, <2 x double>* %682, align 16, !tbaa !6
  %683 = add nuw nsw i64 %656, 432
  %684 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %683
  %685 = bitcast double* %684 to <2 x double>*
  store <2 x double> %664, <2 x double>* %685, align 16, !tbaa !6
  %686 = add nuw nsw i64 %656, 216
  %687 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %686
  %688 = bitcast double* %687 to <2 x double>*
  store <2 x double> %667, <2 x double>* %688, align 16, !tbaa !6
  %689 = add nuw nsw i64 %656, 504
  %690 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %689
  %691 = bitcast double* %690 to <2 x double>*
  store <2 x double> %668, <2 x double>* %691, align 16, !tbaa !6
  %692 = add i64 %656, 2
  %693 = icmp eq i64 %692, 64
  br i1 %693, label %695, label %655, !llvm.loop !12

694:                                              ; preds = %695
  store double %720, double* %21, align 16, !tbaa !6
  store double %724, double* %22, align 8, !tbaa !6
  store double %728, double* %23, align 16, !tbaa !6
  store double %732, double* %24, align 8, !tbaa !6
  store double %736, double* %25, align 16, !tbaa !6
  store double %740, double* %26, align 8, !tbaa !6
  store double %744, double* %27, align 16, !tbaa !6
  store double %748, double* %28, align 8, !tbaa !6
  br label %751

695:                                              ; preds = %655, %695
  %696 = phi i64 [ %749, %695 ], [ 0, %655 ]
  %697 = shl nuw nsw i64 %696, 3
  %698 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %697
  %699 = or i64 %697, 1
  %700 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %699
  %701 = or i64 %697, 2
  %702 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %701
  %703 = or i64 %697, 3
  %704 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %703
  %705 = or i64 %697, 4
  %706 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %705
  %707 = or i64 %697, 5
  %708 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %707
  %709 = or i64 %697, 6
  %710 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %709
  %711 = or i64 %697, 7
  %712 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %711
  %713 = trunc i64 %696 to i32
  %714 = lshr i32 %713, 3
  %715 = and i32 %713, 7
  %716 = mul nuw nsw i32 %714, 72
  %717 = or i32 %716, %715
  %718 = zext i32 %717 to i64
  %719 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %718
  %720 = load double, double* %719, align 8, !tbaa !6
  %721 = add nuw nsw i32 %717, 8
  %722 = zext i32 %721 to i64
  %723 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %722
  %724 = load double, double* %723, align 8, !tbaa !6
  %725 = add nuw nsw i32 %717, 16
  %726 = zext i32 %725 to i64
  %727 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %726
  %728 = load double, double* %727, align 8, !tbaa !6
  %729 = add nuw nsw i32 %717, 24
  %730 = zext i32 %729 to i64
  %731 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %730
  %732 = load double, double* %731, align 8, !tbaa !6
  %733 = add nuw nsw i32 %717, 32
  %734 = zext i32 %733 to i64
  %735 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %734
  %736 = load double, double* %735, align 8, !tbaa !6
  %737 = add nuw nsw i32 %717, 40
  %738 = zext i32 %737 to i64
  %739 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %738
  %740 = load double, double* %739, align 8, !tbaa !6
  %741 = add nuw nsw i32 %717, 48
  %742 = zext i32 %741 to i64
  %743 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %742
  %744 = load double, double* %743, align 8, !tbaa !6
  %745 = add nuw nsw i32 %717, 56
  %746 = zext i32 %745 to i64
  %747 = getelementptr inbounds [576 x double], [576 x double]* %7, i64 0, i64 %746
  %748 = load double, double* %747, align 8, !tbaa !6
  store double %720, double* %698, align 16, !tbaa !6
  store double %724, double* %700, align 8, !tbaa !6
  store double %728, double* %702, align 16, !tbaa !6
  store double %732, double* %704, align 8, !tbaa !6
  store double %736, double* %706, align 16, !tbaa !6
  store double %740, double* %708, align 8, !tbaa !6
  store double %744, double* %710, align 16, !tbaa !6
  store double %748, double* %712, align 8, !tbaa !6
  %749 = add nuw nsw i64 %696, 1
  %750 = icmp eq i64 %749, 64
  br i1 %750, label %694, label %695

751:                                              ; preds = %694, %751
  %752 = phi i64 [ 0, %694 ], [ %887, %751 ]
  %753 = shl nuw nsw i64 %752, 3
  %754 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %753
  %755 = load double, double* %754, align 16, !tbaa !6
  %756 = or i64 %753, 1
  %757 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %756
  %758 = load double, double* %757, align 8, !tbaa !6
  %759 = or i64 %753, 2
  %760 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %759
  %761 = load double, double* %760, align 16, !tbaa !6
  %762 = or i64 %753, 3
  %763 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %762
  %764 = load double, double* %763, align 8, !tbaa !6
  %765 = or i64 %753, 4
  %766 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %765
  %767 = load double, double* %766, align 16, !tbaa !6
  %768 = or i64 %753, 5
  %769 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %768
  %770 = load double, double* %769, align 8, !tbaa !6
  %771 = or i64 %753, 6
  %772 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %771
  %773 = load double, double* %772, align 16, !tbaa !6
  %774 = or i64 %753, 7
  %775 = getelementptr inbounds [512 x double], [512 x double]* %4, i64 0, i64 %774
  %776 = load double, double* %775, align 8, !tbaa !6
  %777 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %753
  %778 = load double, double* %777, align 16, !tbaa !6
  %779 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %756
  %780 = load double, double* %779, align 8, !tbaa !6
  %781 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %759
  %782 = load double, double* %781, align 16, !tbaa !6
  %783 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %762
  %784 = load double, double* %783, align 8, !tbaa !6
  %785 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %765
  %786 = load double, double* %785, align 16, !tbaa !6
  %787 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %768
  %788 = load double, double* %787, align 8, !tbaa !6
  %789 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %771
  %790 = load double, double* %789, align 16, !tbaa !6
  %791 = getelementptr inbounds [512 x double], [512 x double]* %3, i64 0, i64 %774
  %792 = load double, double* %791, align 8, !tbaa !6
  %793 = fadd double %778, %786
  %794 = fadd double %755, %767
  %795 = fsub double %778, %786
  %796 = fsub double %755, %767
  %797 = fadd double %780, %788
  %798 = fadd double %758, %770
  %799 = fsub double %780, %788
  %800 = fsub double %758, %770
  %801 = fadd double %782, %790
  %802 = fadd double %761, %773
  %803 = fsub double %782, %790
  %804 = fsub double %761, %773
  %805 = fadd double %784, %792
  %806 = fadd double %764, %776
  %807 = fsub double %784, %792
  %808 = fsub double %764, %776
  %809 = fadd double %800, %799
  %810 = fmul double %809, 0x3FE6A09E667F3BCD
  %811 = fsub double %800, %799
  %812 = fmul double %811, 0x3FE6A09E667F3BCD
  %813 = fmul double %803, 0.000000e+00
  %814 = fadd double %804, %813
  %815 = fmul double %804, 0.000000e+00
  %816 = fsub double %815, %803
  %817 = fsub double %808, %807
  %818 = fmul double %817, 0x3FE6A09E667F3BCD
  %819 = fneg double %808
  %820 = fsub double %819, %807
  %821 = fmul double %820, 0x3FE6A09E667F3BCD
  %822 = fadd double %793, %801
  %823 = fadd double %794, %802
  %824 = fsub double %793, %801
  %825 = fsub double %794, %802
  %826 = fadd double %797, %805
  %827 = fadd double %798, %806
  %828 = fsub double %797, %805
  %829 = fsub double %798, %806
  %830 = fmul double %828, 0.000000e+00
  %831 = fadd double %829, %830
  %832 = fneg double %828
  %833 = fmul double %829, 0.000000e+00
  %834 = fsub double %832, %833
  %835 = fadd double %822, %826
  %836 = fadd double %823, %827
  %837 = fsub double %822, %826
  %838 = fsub double %823, %827
  %839 = fadd double %824, %831
  %840 = fadd double %825, %834
  %841 = fsub double %824, %831
  %842 = fsub double %825, %834
  %843 = fadd double %795, %814
  %844 = fadd double %796, %816
  %845 = fsub double %795, %814
  %846 = fsub double %796, %816
  %847 = fadd double %810, %818
  %848 = fadd double %812, %821
  %849 = fsub double %810, %818
  %850 = fsub double %812, %821
  %851 = fmul double %849, 0.000000e+00
  %852 = fadd double %850, %851
  %853 = fneg double %849
  %854 = fmul double %850, 0.000000e+00
  %855 = fsub double %853, %854
  %856 = fadd double %843, %847
  %857 = fadd double %844, %848
  %858 = fsub double %843, %847
  %859 = fsub double %844, %848
  %860 = fadd double %845, %852
  %861 = fadd double %846, %855
  %862 = fsub double %845, %852
  %863 = fsub double %846, %855
  %864 = getelementptr inbounds double, double* %0, i64 %752
  store double %835, double* %864, align 8, !tbaa !6
  %865 = add nuw nsw i64 %752, 64
  %866 = getelementptr inbounds double, double* %0, i64 %865
  store double %856, double* %866, align 8, !tbaa !6
  %867 = add nuw nsw i64 %752, 128
  %868 = getelementptr inbounds double, double* %0, i64 %867
  store double %839, double* %868, align 8, !tbaa !6
  %869 = add nuw nsw i64 %752, 192
  %870 = getelementptr inbounds double, double* %0, i64 %869
  store double %860, double* %870, align 8, !tbaa !6
  %871 = add nuw nsw i64 %752, 256
  %872 = getelementptr inbounds double, double* %0, i64 %871
  store double %837, double* %872, align 8, !tbaa !6
  %873 = add nuw nsw i64 %752, 320
  %874 = getelementptr inbounds double, double* %0, i64 %873
  store double %858, double* %874, align 8, !tbaa !6
  %875 = add nuw nsw i64 %752, 384
  %876 = getelementptr inbounds double, double* %0, i64 %875
  store double %841, double* %876, align 8, !tbaa !6
  %877 = add nuw nsw i64 %752, 448
  %878 = getelementptr inbounds double, double* %0, i64 %877
  store double %862, double* %878, align 8, !tbaa !6
  %879 = getelementptr inbounds double, double* %1, i64 %752
  store double %836, double* %879, align 8, !tbaa !6
  %880 = getelementptr inbounds double, double* %1, i64 %865
  store double %857, double* %880, align 8, !tbaa !6
  %881 = getelementptr inbounds double, double* %1, i64 %867
  store double %840, double* %881, align 8, !tbaa !6
  %882 = getelementptr inbounds double, double* %1, i64 %869
  store double %861, double* %882, align 8, !tbaa !6
  %883 = getelementptr inbounds double, double* %1, i64 %871
  store double %838, double* %883, align 8, !tbaa !6
  %884 = getelementptr inbounds double, double* %1, i64 %873
  store double %859, double* %884, align 8, !tbaa !6
  %885 = getelementptr inbounds double, double* %1, i64 %875
  store double %842, double* %885, align 8, !tbaa !6
  %886 = getelementptr inbounds double, double* %1, i64 %877
  store double %863, double* %886, align 8, !tbaa !6
  %887 = add nuw nsw i64 %752, 1
  %888 = icmp eq i64 %887, 64
  br i1 %888, label %889, label %751

889:                                              ; preds = %751
  call void @llvm.lifetime.end.p0i8(i64 4608, i8* nonnull %12) #6
  call void @llvm.lifetime.end.p0i8(i64 64, i8* nonnull %11) #6
  call void @llvm.lifetime.end.p0i8(i64 64, i8* nonnull %10) #6
  call void @llvm.lifetime.end.p0i8(i64 4096, i8* nonnull %9) #6
  call void @llvm.lifetime.end.p0i8(i64 4096, i8* nonnull %8) #6
  ret void
}

attributes #0 = { alwaysinline nofree nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nofree nounwind optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { alwaysinline nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind optsize }
attributes #6 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"double", !4, i64 0}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.isvectorized", i32 1}
!10 = distinct !{!10, !9}
!11 = distinct !{!11, !9}
!12 = distinct !{!12, !9}
