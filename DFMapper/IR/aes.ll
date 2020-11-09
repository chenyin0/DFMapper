; ModuleID = 'aes.c'
source_filename = "aes.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.aes256_context = type { [32 x i8], [32 x i8], [32 x i8] }

@sbox = dso_local local_unnamed_addr constant [256 x i8] c"c|w{\F2ko\C50\01g+\FE\D7\ABv\CA\82\C9}\FAYG\F0\AD\D4\A2\AF\9C\A4r\C0\B7\FD\93&6?\F7\CC4\A5\E5\F1q\D81\15\04\C7#\C3\18\96\05\9A\07\12\80\E2\EB'\B2u\09\83,\1A\1BnZ\A0R;\D6\B3)\E3/\84S\D1\00\ED \FC\B1[j\CB\BE9JLX\CF\D0\EF\AA\FBCM3\85E\F9\02\7FP<\9F\A8Q\A3@\8F\92\9D8\F5\BC\B6\DA!\10\FF\F3\D2\CD\0C\13\EC_\97D\17\C4\A7~=d]\19s`\81O\DC\22*\90\88F\EE\B8\14\DE^\0B\DB\E02:\0AI\06$\\\C2\D3\ACb\91\95\E4y\E7\C87m\8D\D5N\A9lV\F4\EAez\AE\08\BAx%.\1C\A6\B4\C6\E8\DDt\1FK\BD\8B\8Ap>\B5fH\03\F6\0Ea5W\B9\86\C1\1D\9E\E1\F8\98\11i\D9\8E\94\9B\1E\87\E9\CEU(\DF\8C\A1\89\0D\BF\E6BhA\99-\0F\B0T\BB\16", align 16

; Function Attrs: alwaysinline norecurse nounwind optsize readnone uwtable
define dso_local zeroext i8 @rj_xtime(i8 zeroext %0) local_unnamed_addr #0 {
  %2 = icmp sgt i8 %0, -1
  %3 = shl i8 %0, 1
  %4 = xor i8 %3, 27
  %5 = select i1 %2, i8 %3, i8 %4
  ret i8 %5
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @aes_subBytes(i8* nocapture %0) local_unnamed_addr #1 {
  br label %2

2:                                                ; preds = %1, %2
  %3 = phi i64 [ 15, %1 ], [ %9, %2 ]
  %4 = getelementptr inbounds i8, i8* %0, i64 %3
  %5 = load i8, i8* %4, align 1, !tbaa !2
  %6 = zext i8 %5 to i64
  %7 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %6
  %8 = load i8, i8* %7, align 1, !tbaa !2
  store i8 %8, i8* %4, align 1, !tbaa !2
  %9 = add nsw i64 %3, -1
  %10 = icmp eq i64 %3, 0
  br i1 %10, label %11, label %2

11:                                               ; preds = %2
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @aes_addRoundKey(i8* nocapture %0, i8* nocapture readonly %1) local_unnamed_addr #1 {
  br label %3

3:                                                ; preds = %2, %3
  %4 = phi i64 [ 15, %2 ], [ %10, %3 ]
  %5 = getelementptr inbounds i8, i8* %1, i64 %4
  %6 = load i8, i8* %5, align 1, !tbaa !2
  %7 = getelementptr inbounds i8, i8* %0, i64 %4
  %8 = load i8, i8* %7, align 1, !tbaa !2
  %9 = xor i8 %8, %6
  store i8 %9, i8* %7, align 1, !tbaa !2
  %10 = add nsw i64 %4, -1
  %11 = icmp eq i64 %4, 0
  br i1 %11, label %12, label %3

12:                                               ; preds = %3
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @aes_addRoundKey_cpy(i8* nocapture %0, i8* nocapture readonly %1, i8* nocapture %2) local_unnamed_addr #1 {
  br label %4

4:                                                ; preds = %3, %4
  %5 = phi i64 [ 15, %3 ], [ %16, %4 ]
  %6 = getelementptr inbounds i8, i8* %1, i64 %5
  %7 = load i8, i8* %6, align 1, !tbaa !2
  %8 = getelementptr inbounds i8, i8* %2, i64 %5
  store i8 %7, i8* %8, align 1, !tbaa !2
  %9 = getelementptr inbounds i8, i8* %0, i64 %5
  %10 = load i8, i8* %9, align 1, !tbaa !2
  %11 = xor i8 %10, %7
  store i8 %11, i8* %9, align 1, !tbaa !2
  %12 = add nuw nsw i64 %5, 16
  %13 = getelementptr inbounds i8, i8* %1, i64 %12
  %14 = load i8, i8* %13, align 1, !tbaa !2
  %15 = getelementptr inbounds i8, i8* %2, i64 %12
  store i8 %14, i8* %15, align 1, !tbaa !2
  %16 = add nsw i64 %5, -1
  %17 = icmp eq i64 %5, 0
  br i1 %17, label %18, label %4

18:                                               ; preds = %4
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @aes_shiftRows(i8* nocapture %0) local_unnamed_addr #1 {
  %2 = getelementptr inbounds i8, i8* %0, i64 1
  %3 = load i8, i8* %2, align 1, !tbaa !2
  %4 = getelementptr inbounds i8, i8* %0, i64 5
  %5 = load i8, i8* %4, align 1, !tbaa !2
  store i8 %5, i8* %2, align 1, !tbaa !2
  %6 = getelementptr inbounds i8, i8* %0, i64 9
  %7 = load i8, i8* %6, align 1, !tbaa !2
  store i8 %7, i8* %4, align 1, !tbaa !2
  %8 = getelementptr inbounds i8, i8* %0, i64 13
  %9 = load i8, i8* %8, align 1, !tbaa !2
  store i8 %9, i8* %6, align 1, !tbaa !2
  store i8 %3, i8* %8, align 1, !tbaa !2
  %10 = getelementptr inbounds i8, i8* %0, i64 10
  %11 = load i8, i8* %10, align 1, !tbaa !2
  %12 = getelementptr inbounds i8, i8* %0, i64 2
  %13 = load i8, i8* %12, align 1, !tbaa !2
  store i8 %13, i8* %10, align 1, !tbaa !2
  store i8 %11, i8* %12, align 1, !tbaa !2
  %14 = getelementptr inbounds i8, i8* %0, i64 3
  %15 = load i8, i8* %14, align 1, !tbaa !2
  %16 = getelementptr inbounds i8, i8* %0, i64 15
  %17 = load i8, i8* %16, align 1, !tbaa !2
  store i8 %17, i8* %14, align 1, !tbaa !2
  %18 = getelementptr inbounds i8, i8* %0, i64 11
  %19 = load i8, i8* %18, align 1, !tbaa !2
  store i8 %19, i8* %16, align 1, !tbaa !2
  %20 = getelementptr inbounds i8, i8* %0, i64 7
  %21 = load i8, i8* %20, align 1, !tbaa !2
  store i8 %21, i8* %18, align 1, !tbaa !2
  store i8 %15, i8* %20, align 1, !tbaa !2
  %22 = getelementptr inbounds i8, i8* %0, i64 14
  %23 = load i8, i8* %22, align 1, !tbaa !2
  %24 = getelementptr inbounds i8, i8* %0, i64 6
  %25 = load i8, i8* %24, align 1, !tbaa !2
  store i8 %25, i8* %22, align 1, !tbaa !2
  store i8 %23, i8* %24, align 1, !tbaa !2
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @aes_mixColumns(i8* nocapture %0) local_unnamed_addr #1 {
  br label %2

2:                                                ; preds = %1, %2
  %3 = phi i64 [ 0, %1 ], [ %44, %2 ]
  %4 = getelementptr inbounds i8, i8* %0, i64 %3
  %5 = load i8, i8* %4, align 1, !tbaa !2
  %6 = or i64 %3, 1
  %7 = getelementptr inbounds i8, i8* %0, i64 %6
  %8 = load i8, i8* %7, align 1, !tbaa !2
  %9 = or i64 %3, 2
  %10 = getelementptr inbounds i8, i8* %0, i64 %9
  %11 = load i8, i8* %10, align 1, !tbaa !2
  %12 = or i64 %3, 3
  %13 = getelementptr inbounds i8, i8* %0, i64 %12
  %14 = load i8, i8* %13, align 1, !tbaa !2
  %15 = xor i8 %8, %5
  %16 = xor i8 %15, %11
  %17 = xor i8 %16, %14
  %18 = icmp sgt i8 %15, -1
  %19 = shl i8 %15, 1
  %20 = xor i8 %19, 27
  %21 = select i1 %18, i8 %19, i8 %20
  %22 = xor i8 %17, %5
  %23 = xor i8 %22, %21
  store i8 %23, i8* %4, align 1, !tbaa !2
  %24 = xor i8 %11, %8
  %25 = icmp sgt i8 %24, -1
  %26 = shl i8 %24, 1
  %27 = xor i8 %26, 27
  %28 = select i1 %25, i8 %26, i8 %27
  %29 = xor i8 %17, %8
  %30 = xor i8 %29, %28
  store i8 %30, i8* %7, align 1, !tbaa !2
  %31 = xor i8 %14, %11
  %32 = icmp sgt i8 %31, -1
  %33 = shl i8 %31, 1
  %34 = xor i8 %33, 27
  %35 = select i1 %32, i8 %33, i8 %34
  %36 = xor i8 %15, %14
  %37 = xor i8 %36, %35
  store i8 %37, i8* %10, align 1, !tbaa !2
  %38 = xor i8 %14, %5
  %39 = icmp sgt i8 %38, -1
  %40 = shl i8 %38, 1
  %41 = xor i8 %40, 27
  %42 = select i1 %39, i8 %40, i8 %41
  %43 = xor i8 %42, %16
  store i8 %43, i8* %13, align 1, !tbaa !2
  %44 = add nuw nsw i64 %3, 4
  %45 = icmp ult i64 %3, 12
  br i1 %45, label %2, label %46

46:                                               ; preds = %2
  ret void
}

; Function Attrs: alwaysinline nofree norecurse nounwind optsize uwtable
define dso_local void @aes_expandEncKey(i8* %0, i8* nocapture %1) local_unnamed_addr #1 {
  %3 = getelementptr inbounds i8, i8* %0, i64 29
  %4 = load i8, i8* %3, align 1, !tbaa !2
  %5 = zext i8 %4 to i64
  %6 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %5
  %7 = load i8, i8* %6, align 1, !tbaa !2
  %8 = load i8, i8* %1, align 1, !tbaa !2
  %9 = xor i8 %8, %7
  %10 = load i8, i8* %0, align 1, !tbaa !2
  %11 = xor i8 %9, %10
  store i8 %11, i8* %0, align 1, !tbaa !2
  %12 = getelementptr inbounds i8, i8* %0, i64 30
  %13 = load i8, i8* %12, align 1, !tbaa !2
  %14 = zext i8 %13 to i64
  %15 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %14
  %16 = load i8, i8* %15, align 1, !tbaa !2
  %17 = getelementptr inbounds i8, i8* %0, i64 1
  %18 = load i8, i8* %17, align 1, !tbaa !2
  %19 = xor i8 %18, %16
  store i8 %19, i8* %17, align 1, !tbaa !2
  %20 = getelementptr inbounds i8, i8* %0, i64 31
  %21 = load i8, i8* %20, align 1, !tbaa !2
  %22 = zext i8 %21 to i64
  %23 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %22
  %24 = load i8, i8* %23, align 1, !tbaa !2
  %25 = getelementptr inbounds i8, i8* %0, i64 2
  %26 = load i8, i8* %25, align 1, !tbaa !2
  %27 = xor i8 %26, %24
  store i8 %27, i8* %25, align 1, !tbaa !2
  %28 = getelementptr inbounds i8, i8* %0, i64 28
  %29 = load i8, i8* %28, align 1, !tbaa !2
  %30 = zext i8 %29 to i64
  %31 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %30
  %32 = load i8, i8* %31, align 1, !tbaa !2
  %33 = getelementptr inbounds i8, i8* %0, i64 3
  %34 = load i8, i8* %33, align 1, !tbaa !2
  %35 = xor i8 %34, %32
  store i8 %35, i8* %33, align 1, !tbaa !2
  %36 = load i8, i8* %1, align 1, !tbaa !2
  %37 = shl i8 %36, 1
  %38 = ashr i8 %36, 7
  %39 = and i8 %38, 27
  %40 = xor i8 %39, %37
  store i8 %40, i8* %1, align 1, !tbaa !2
  %41 = load i8, i8* %0, align 1, !tbaa !2
  br label %42

42:                                               ; preds = %2, %42
  %43 = phi i8 [ %41, %2 ], [ %47, %42 ]
  %44 = phi i64 [ 4, %2 ], [ %69, %42 ]
  %45 = getelementptr inbounds i8, i8* %0, i64 %44
  %46 = load i8, i8* %45, align 1, !tbaa !2
  %47 = xor i8 %46, %43
  store i8 %47, i8* %45, align 1, !tbaa !2
  %48 = add nsw i64 %44, -3
  %49 = getelementptr inbounds i8, i8* %0, i64 %48
  %50 = load i8, i8* %49, align 1, !tbaa !2
  %51 = or i64 %44, 1
  %52 = getelementptr inbounds i8, i8* %0, i64 %51
  %53 = load i8, i8* %52, align 1, !tbaa !2
  %54 = xor i8 %53, %50
  store i8 %54, i8* %52, align 1, !tbaa !2
  %55 = add nsw i64 %44, -2
  %56 = getelementptr inbounds i8, i8* %0, i64 %55
  %57 = load i8, i8* %56, align 1, !tbaa !2
  %58 = or i64 %44, 2
  %59 = getelementptr inbounds i8, i8* %0, i64 %58
  %60 = load i8, i8* %59, align 1, !tbaa !2
  %61 = xor i8 %60, %57
  store i8 %61, i8* %59, align 1, !tbaa !2
  %62 = add nsw i64 %44, -1
  %63 = getelementptr inbounds i8, i8* %0, i64 %62
  %64 = load i8, i8* %63, align 1, !tbaa !2
  %65 = or i64 %44, 3
  %66 = getelementptr inbounds i8, i8* %0, i64 %65
  %67 = load i8, i8* %66, align 1, !tbaa !2
  %68 = xor i8 %67, %64
  store i8 %68, i8* %66, align 1, !tbaa !2
  %69 = add nuw nsw i64 %44, 4
  %70 = icmp ult i64 %44, 12
  br i1 %70, label %42, label %71

71:                                               ; preds = %42
  %72 = getelementptr inbounds i8, i8* %0, i64 12
  %73 = load i8, i8* %72, align 1, !tbaa !2
  %74 = zext i8 %73 to i64
  %75 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %74
  %76 = load i8, i8* %75, align 1, !tbaa !2
  %77 = getelementptr inbounds i8, i8* %0, i64 16
  %78 = load i8, i8* %77, align 1, !tbaa !2
  %79 = xor i8 %78, %76
  store i8 %79, i8* %77, align 1, !tbaa !2
  %80 = getelementptr inbounds i8, i8* %0, i64 13
  %81 = load i8, i8* %80, align 1, !tbaa !2
  %82 = zext i8 %81 to i64
  %83 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %82
  %84 = load i8, i8* %83, align 1, !tbaa !2
  %85 = getelementptr inbounds i8, i8* %0, i64 17
  %86 = load i8, i8* %85, align 1, !tbaa !2
  %87 = xor i8 %86, %84
  store i8 %87, i8* %85, align 1, !tbaa !2
  %88 = getelementptr inbounds i8, i8* %0, i64 14
  %89 = load i8, i8* %88, align 1, !tbaa !2
  %90 = zext i8 %89 to i64
  %91 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %90
  %92 = load i8, i8* %91, align 1, !tbaa !2
  %93 = getelementptr inbounds i8, i8* %0, i64 18
  %94 = load i8, i8* %93, align 1, !tbaa !2
  %95 = xor i8 %94, %92
  store i8 %95, i8* %93, align 1, !tbaa !2
  %96 = getelementptr inbounds i8, i8* %0, i64 15
  %97 = load i8, i8* %96, align 1, !tbaa !2
  %98 = zext i8 %97 to i64
  %99 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %98
  %100 = load i8, i8* %99, align 1, !tbaa !2
  %101 = getelementptr inbounds i8, i8* %0, i64 19
  %102 = load i8, i8* %101, align 1, !tbaa !2
  %103 = xor i8 %102, %100
  store i8 %103, i8* %101, align 1, !tbaa !2
  br label %104

104:                                              ; preds = %104, %71
  %105 = phi i8 [ %79, %71 ], [ %109, %104 ]
  %106 = phi i64 [ 20, %71 ], [ %132, %104 ]
  %107 = getelementptr inbounds i8, i8* %0, i64 %106
  %108 = load i8, i8* %107, align 1, !tbaa !2
  %109 = xor i8 %108, %105
  store i8 %109, i8* %107, align 1, !tbaa !2
  %110 = add nsw i64 %106, -3
  %111 = getelementptr inbounds i8, i8* %0, i64 %110
  %112 = load i8, i8* %111, align 1, !tbaa !2
  %113 = or i64 %106, 1
  %114 = getelementptr inbounds i8, i8* %0, i64 %113
  %115 = load i8, i8* %114, align 1, !tbaa !2
  %116 = xor i8 %115, %112
  store i8 %116, i8* %114, align 1, !tbaa !2
  %117 = add nsw i64 %106, -2
  %118 = getelementptr inbounds i8, i8* %0, i64 %117
  %119 = load i8, i8* %118, align 1, !tbaa !2
  %120 = or i64 %106, 2
  %121 = getelementptr inbounds i8, i8* %0, i64 %120
  %122 = load i8, i8* %121, align 1, !tbaa !2
  %123 = xor i8 %122, %119
  store i8 %123, i8* %121, align 1, !tbaa !2
  %124 = add nsw i64 %106, -1
  %125 = getelementptr inbounds i8, i8* %0, i64 %124
  %126 = load i8, i8* %125, align 1, !tbaa !2
  %127 = or i64 %106, 3
  %128 = getelementptr inbounds i8, i8* %0, i64 %127
  %129 = load i8, i8* %128, align 1, !tbaa !2
  %130 = xor i8 %129, %126
  store i8 %130, i8* %128, align 1, !tbaa !2
  %131 = icmp ult i64 %106, 28
  %132 = add nuw nsw i64 %106, 4
  br i1 %131, label %104, label %133

133:                                              ; preds = %104
  ret void
}

; Function Attrs: nounwind optsize uwtable
define dso_local void @aes256_encrypt_ecb(%struct.aes256_context* %0, i8* nocapture readonly %1, i8* %2) local_unnamed_addr #2 {
  br label %21

4:                                                ; preds = %21
  %5 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 0
  %6 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 29
  %7 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 30
  %8 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 1
  %9 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 31
  %10 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 2
  %11 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 28
  %12 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 3
  %13 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 12
  %14 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 16
  %15 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 13
  %16 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 17
  %17 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 14
  %18 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 18
  %19 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 15
  %20 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 19
  br label %33

21:                                               ; preds = %3, %21
  %22 = phi i64 [ 0, %3 ], [ %27, %21 ]
  %23 = getelementptr inbounds i8, i8* %1, i64 %22
  %24 = load i8, i8* %23, align 1, !tbaa !2
  %25 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %22
  store i8 %24, i8* %25, align 1, !tbaa !2
  %26 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 1, i64 %22
  store i8 %24, i8* %26, align 1, !tbaa !2
  %27 = add nuw nsw i64 %22, 1
  %28 = icmp eq i64 %27, 32
  br i1 %28, label %4, label %21

29:                                               ; preds = %118
  %30 = xor i8 %62, %93
  %31 = add nsw i8 %34, -1
  %32 = icmp eq i8 %31, 0
  br i1 %32, label %147, label %33

33:                                               ; preds = %4, %29
  %34 = phi i8 [ 7, %4 ], [ %31, %29 ]
  %35 = phi i8 [ 1, %4 ], [ %30, %29 ]
  %36 = load i8, i8* %6, align 1, !tbaa !2
  %37 = zext i8 %36 to i64
  %38 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %37
  %39 = load i8, i8* %38, align 1, !tbaa !2
  %40 = xor i8 %39, %35
  %41 = load i8, i8* %5, align 1, !tbaa !2
  %42 = xor i8 %40, %41
  store i8 %42, i8* %5, align 1, !tbaa !2
  %43 = load i8, i8* %7, align 1, !tbaa !2
  %44 = zext i8 %43 to i64
  %45 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %44
  %46 = load i8, i8* %45, align 1, !tbaa !2
  %47 = load i8, i8* %8, align 1, !tbaa !2
  %48 = xor i8 %47, %46
  store i8 %48, i8* %8, align 1, !tbaa !2
  %49 = load i8, i8* %9, align 1, !tbaa !2
  %50 = zext i8 %49 to i64
  %51 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %50
  %52 = load i8, i8* %51, align 1, !tbaa !2
  %53 = load i8, i8* %10, align 1, !tbaa !2
  %54 = xor i8 %53, %52
  store i8 %54, i8* %10, align 1, !tbaa !2
  %55 = load i8, i8* %11, align 1, !tbaa !2
  %56 = zext i8 %55 to i64
  %57 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %56
  %58 = load i8, i8* %57, align 1, !tbaa !2
  %59 = load i8, i8* %12, align 1, !tbaa !2
  %60 = xor i8 %59, %58
  store i8 %60, i8* %12, align 1, !tbaa !2
  %61 = ashr i8 %35, 7
  %62 = and i8 %61, 27
  br label %63

63:                                               ; preds = %63, %33
  %64 = phi i8 [ %42, %33 ], [ %68, %63 ]
  %65 = phi i64 [ 4, %33 ], [ %91, %63 ]
  %66 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %65
  %67 = load i8, i8* %66, align 1, !tbaa !2
  %68 = xor i8 %67, %64
  store i8 %68, i8* %66, align 1, !tbaa !2
  %69 = add nsw i64 %65, -3
  %70 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %69
  %71 = load i8, i8* %70, align 1, !tbaa !2
  %72 = or i64 %65, 1
  %73 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %72
  %74 = load i8, i8* %73, align 1, !tbaa !2
  %75 = xor i8 %74, %71
  store i8 %75, i8* %73, align 1, !tbaa !2
  %76 = add nsw i64 %65, -2
  %77 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %76
  %78 = load i8, i8* %77, align 1, !tbaa !2
  %79 = or i64 %65, 2
  %80 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %79
  %81 = load i8, i8* %80, align 1, !tbaa !2
  %82 = xor i8 %81, %78
  store i8 %82, i8* %80, align 1, !tbaa !2
  %83 = add nsw i64 %65, -1
  %84 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %83
  %85 = load i8, i8* %84, align 1, !tbaa !2
  %86 = or i64 %65, 3
  %87 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %86
  %88 = load i8, i8* %87, align 1, !tbaa !2
  %89 = xor i8 %88, %85
  store i8 %89, i8* %87, align 1, !tbaa !2
  %90 = icmp ult i64 %65, 12
  %91 = add nuw nsw i64 %65, 4
  br i1 %90, label %63, label %92

92:                                               ; preds = %63
  %93 = shl i8 %35, 1
  %94 = load i8, i8* %13, align 1, !tbaa !2
  %95 = zext i8 %94 to i64
  %96 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %95
  %97 = load i8, i8* %96, align 1, !tbaa !2
  %98 = load i8, i8* %14, align 1, !tbaa !2
  %99 = xor i8 %98, %97
  store i8 %99, i8* %14, align 1, !tbaa !2
  %100 = load i8, i8* %15, align 1, !tbaa !2
  %101 = zext i8 %100 to i64
  %102 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %101
  %103 = load i8, i8* %102, align 1, !tbaa !2
  %104 = load i8, i8* %16, align 1, !tbaa !2
  %105 = xor i8 %104, %103
  store i8 %105, i8* %16, align 1, !tbaa !2
  %106 = load i8, i8* %17, align 1, !tbaa !2
  %107 = zext i8 %106 to i64
  %108 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %107
  %109 = load i8, i8* %108, align 1, !tbaa !2
  %110 = load i8, i8* %18, align 1, !tbaa !2
  %111 = xor i8 %110, %109
  store i8 %111, i8* %18, align 1, !tbaa !2
  %112 = load i8, i8* %19, align 1, !tbaa !2
  %113 = zext i8 %112 to i64
  %114 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %113
  %115 = load i8, i8* %114, align 1, !tbaa !2
  %116 = load i8, i8* %20, align 1, !tbaa !2
  %117 = xor i8 %116, %115
  store i8 %117, i8* %20, align 1, !tbaa !2
  br label %118

118:                                              ; preds = %118, %92
  %119 = phi i8 [ %99, %92 ], [ %123, %118 ]
  %120 = phi i64 [ 20, %92 ], [ %146, %118 ]
  %121 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %120
  %122 = load i8, i8* %121, align 1, !tbaa !2
  %123 = xor i8 %122, %119
  store i8 %123, i8* %121, align 1, !tbaa !2
  %124 = add nsw i64 %120, -3
  %125 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %124
  %126 = load i8, i8* %125, align 1, !tbaa !2
  %127 = or i64 %120, 1
  %128 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %127
  %129 = load i8, i8* %128, align 1, !tbaa !2
  %130 = xor i8 %129, %126
  store i8 %130, i8* %128, align 1, !tbaa !2
  %131 = add nsw i64 %120, -2
  %132 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %131
  %133 = load i8, i8* %132, align 1, !tbaa !2
  %134 = or i64 %120, 2
  %135 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %134
  %136 = load i8, i8* %135, align 1, !tbaa !2
  %137 = xor i8 %136, %133
  store i8 %137, i8* %135, align 1, !tbaa !2
  %138 = add nsw i64 %120, -1
  %139 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %138
  %140 = load i8, i8* %139, align 1, !tbaa !2
  %141 = or i64 %120, 3
  %142 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 2, i64 %141
  %143 = load i8, i8* %142, align 1, !tbaa !2
  %144 = xor i8 %143, %140
  store i8 %144, i8* %142, align 1, !tbaa !2
  %145 = icmp ult i64 %120, 28
  %146 = add nuw nsw i64 %120, 4
  br i1 %145, label %118, label %29

147:                                              ; preds = %29, %147
  %148 = phi i64 [ %159, %147 ], [ 15, %29 ]
  %149 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 1, i64 %148
  %150 = load i8, i8* %149, align 1, !tbaa !2
  %151 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %148
  store i8 %150, i8* %151, align 1, !tbaa !2
  %152 = getelementptr inbounds i8, i8* %2, i64 %148
  %153 = load i8, i8* %152, align 1, !tbaa !2
  %154 = xor i8 %153, %150
  store i8 %154, i8* %152, align 1, !tbaa !2
  %155 = add nuw nsw i64 %148, 16
  %156 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 1, i64 %155
  %157 = load i8, i8* %156, align 1, !tbaa !2
  %158 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %155
  store i8 %157, i8* %158, align 1, !tbaa !2
  %159 = add nsw i64 %148, -1
  %160 = icmp eq i64 %148, 0
  br i1 %160, label %161, label %147

161:                                              ; preds = %147
  %162 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 0
  %163 = getelementptr inbounds i8, i8* %2, i64 1
  %164 = getelementptr inbounds i8, i8* %2, i64 5
  %165 = getelementptr inbounds i8, i8* %2, i64 9
  %166 = getelementptr inbounds i8, i8* %2, i64 13
  %167 = getelementptr inbounds i8, i8* %2, i64 10
  %168 = getelementptr inbounds i8, i8* %2, i64 2
  %169 = getelementptr inbounds i8, i8* %2, i64 3
  %170 = getelementptr inbounds i8, i8* %2, i64 15
  %171 = getelementptr inbounds i8, i8* %2, i64 11
  %172 = getelementptr inbounds i8, i8* %2, i64 7
  %173 = getelementptr inbounds i8, i8* %2, i64 14
  %174 = getelementptr inbounds i8, i8* %2, i64 6
  %175 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 16
  %176 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 29
  %177 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 30
  %178 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 1
  %179 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 31
  %180 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 2
  %181 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 28
  %182 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 3
  %183 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 12
  %184 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 13
  %185 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 17
  %186 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 14
  %187 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 18
  %188 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 15
  %189 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 19
  br label %190

190:                                              ; preds = %161, %394
  %191 = phi i8 [ 1, %161 ], [ %396, %394 ]
  %192 = phi i8 [ 1, %161 ], [ %395, %394 ]
  br label %193

193:                                              ; preds = %190, %193
  %194 = phi i64 [ %200, %193 ], [ 15, %190 ]
  %195 = getelementptr inbounds i8, i8* %2, i64 %194
  %196 = load i8, i8* %195, align 1, !tbaa !2
  %197 = zext i8 %196 to i64
  %198 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %197
  %199 = load i8, i8* %198, align 1, !tbaa !2
  store i8 %199, i8* %195, align 1, !tbaa !2
  %200 = add nsw i64 %194, -1
  %201 = icmp eq i64 %194, 0
  br i1 %201, label %202, label %193

202:                                              ; preds = %193
  %203 = load i8, i8* %163, align 1, !tbaa !2
  %204 = load i8, i8* %164, align 1, !tbaa !2
  store i8 %204, i8* %163, align 1, !tbaa !2
  %205 = load i8, i8* %165, align 1, !tbaa !2
  store i8 %205, i8* %164, align 1, !tbaa !2
  %206 = load i8, i8* %166, align 1, !tbaa !2
  store i8 %206, i8* %165, align 1, !tbaa !2
  store i8 %203, i8* %166, align 1, !tbaa !2
  %207 = load i8, i8* %167, align 1, !tbaa !2
  %208 = load i8, i8* %168, align 1, !tbaa !2
  store i8 %208, i8* %167, align 1, !tbaa !2
  store i8 %207, i8* %168, align 1, !tbaa !2
  %209 = load i8, i8* %169, align 1, !tbaa !2
  %210 = load i8, i8* %170, align 1, !tbaa !2
  store i8 %210, i8* %169, align 1, !tbaa !2
  %211 = load i8, i8* %171, align 1, !tbaa !2
  store i8 %211, i8* %170, align 1, !tbaa !2
  %212 = load i8, i8* %172, align 1, !tbaa !2
  store i8 %212, i8* %171, align 1, !tbaa !2
  store i8 %209, i8* %172, align 1, !tbaa !2
  %213 = load i8, i8* %173, align 1, !tbaa !2
  %214 = load i8, i8* %174, align 1, !tbaa !2
  store i8 %214, i8* %173, align 1, !tbaa !2
  store i8 %213, i8* %174, align 1, !tbaa !2
  br label %215

215:                                              ; preds = %215, %202
  %216 = phi i64 [ 0, %202 ], [ %257, %215 ]
  %217 = getelementptr inbounds i8, i8* %2, i64 %216
  %218 = load i8, i8* %217, align 1, !tbaa !2
  %219 = or i64 %216, 1
  %220 = getelementptr inbounds i8, i8* %2, i64 %219
  %221 = load i8, i8* %220, align 1, !tbaa !2
  %222 = or i64 %216, 2
  %223 = getelementptr inbounds i8, i8* %2, i64 %222
  %224 = load i8, i8* %223, align 1, !tbaa !2
  %225 = or i64 %216, 3
  %226 = getelementptr inbounds i8, i8* %2, i64 %225
  %227 = load i8, i8* %226, align 1, !tbaa !2
  %228 = xor i8 %221, %218
  %229 = xor i8 %228, %224
  %230 = xor i8 %229, %227
  %231 = icmp sgt i8 %228, -1
  %232 = shl i8 %228, 1
  %233 = xor i8 %232, 27
  %234 = select i1 %231, i8 %232, i8 %233
  %235 = xor i8 %230, %218
  %236 = xor i8 %235, %234
  store i8 %236, i8* %217, align 1, !tbaa !2
  %237 = xor i8 %224, %221
  %238 = icmp sgt i8 %237, -1
  %239 = shl i8 %237, 1
  %240 = xor i8 %239, 27
  %241 = select i1 %238, i8 %239, i8 %240
  %242 = xor i8 %230, %221
  %243 = xor i8 %242, %241
  store i8 %243, i8* %220, align 1, !tbaa !2
  %244 = xor i8 %227, %224
  %245 = icmp sgt i8 %244, -1
  %246 = shl i8 %244, 1
  %247 = xor i8 %246, 27
  %248 = select i1 %245, i8 %246, i8 %247
  %249 = xor i8 %227, %228
  %250 = xor i8 %249, %248
  store i8 %250, i8* %223, align 1, !tbaa !2
  %251 = xor i8 %227, %218
  %252 = icmp sgt i8 %251, -1
  %253 = shl i8 %251, 1
  %254 = xor i8 %253, 27
  %255 = select i1 %252, i8 %253, i8 %254
  %256 = xor i8 %255, %229
  store i8 %256, i8* %226, align 1, !tbaa !2
  %257 = add nuw nsw i64 %216, 4
  %258 = icmp ult i64 %216, 12
  br i1 %258, label %215, label %259

259:                                              ; preds = %215
  %260 = and i8 %191, 1
  %261 = icmp eq i8 %260, 0
  br i1 %261, label %271, label %262

262:                                              ; preds = %259, %262
  %263 = phi i64 [ %269, %262 ], [ 15, %259 ]
  %264 = getelementptr inbounds i8, i8* %175, i64 %263
  %265 = load i8, i8* %264, align 1, !tbaa !2
  %266 = getelementptr inbounds i8, i8* %2, i64 %263
  %267 = load i8, i8* %266, align 1, !tbaa !2
  %268 = xor i8 %267, %265
  store i8 %268, i8* %266, align 1, !tbaa !2
  %269 = add nsw i64 %263, -1
  %270 = icmp eq i64 %263, 0
  br i1 %270, label %394, label %262

271:                                              ; preds = %259
  %272 = load i8, i8* %176, align 1, !tbaa !2
  %273 = zext i8 %272 to i64
  %274 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %273
  %275 = load i8, i8* %274, align 1, !tbaa !2
  %276 = xor i8 %275, %192
  %277 = load i8, i8* %162, align 1, !tbaa !2
  %278 = xor i8 %276, %277
  store i8 %278, i8* %162, align 1, !tbaa !2
  %279 = load i8, i8* %177, align 1, !tbaa !2
  %280 = zext i8 %279 to i64
  %281 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %280
  %282 = load i8, i8* %281, align 1, !tbaa !2
  %283 = load i8, i8* %178, align 1, !tbaa !2
  %284 = xor i8 %283, %282
  store i8 %284, i8* %178, align 1, !tbaa !2
  %285 = load i8, i8* %179, align 1, !tbaa !2
  %286 = zext i8 %285 to i64
  %287 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %286
  %288 = load i8, i8* %287, align 1, !tbaa !2
  %289 = load i8, i8* %180, align 1, !tbaa !2
  %290 = xor i8 %289, %288
  store i8 %290, i8* %180, align 1, !tbaa !2
  %291 = load i8, i8* %181, align 1, !tbaa !2
  %292 = zext i8 %291 to i64
  %293 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %292
  %294 = load i8, i8* %293, align 1, !tbaa !2
  %295 = load i8, i8* %182, align 1, !tbaa !2
  %296 = xor i8 %295, %294
  store i8 %296, i8* %182, align 1, !tbaa !2
  %297 = ashr i8 %192, 7
  %298 = and i8 %297, 27
  br label %299

299:                                              ; preds = %299, %271
  %300 = phi i8 [ %278, %271 ], [ %304, %299 ]
  %301 = phi i64 [ 4, %271 ], [ %327, %299 ]
  %302 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %301
  %303 = load i8, i8* %302, align 1, !tbaa !2
  %304 = xor i8 %303, %300
  store i8 %304, i8* %302, align 1, !tbaa !2
  %305 = add nsw i64 %301, -3
  %306 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %305
  %307 = load i8, i8* %306, align 1, !tbaa !2
  %308 = or i64 %301, 1
  %309 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %308
  %310 = load i8, i8* %309, align 1, !tbaa !2
  %311 = xor i8 %310, %307
  store i8 %311, i8* %309, align 1, !tbaa !2
  %312 = add nsw i64 %301, -2
  %313 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %312
  %314 = load i8, i8* %313, align 1, !tbaa !2
  %315 = or i64 %301, 2
  %316 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %315
  %317 = load i8, i8* %316, align 1, !tbaa !2
  %318 = xor i8 %317, %314
  store i8 %318, i8* %316, align 1, !tbaa !2
  %319 = add nsw i64 %301, -1
  %320 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %319
  %321 = load i8, i8* %320, align 1, !tbaa !2
  %322 = or i64 %301, 3
  %323 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %322
  %324 = load i8, i8* %323, align 1, !tbaa !2
  %325 = xor i8 %324, %321
  store i8 %325, i8* %323, align 1, !tbaa !2
  %326 = icmp ult i64 %301, 12
  %327 = add nuw nsw i64 %301, 4
  br i1 %326, label %299, label %328

328:                                              ; preds = %299
  %329 = shl i8 %192, 1
  %330 = load i8, i8* %183, align 1, !tbaa !2
  %331 = zext i8 %330 to i64
  %332 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %331
  %333 = load i8, i8* %332, align 1, !tbaa !2
  %334 = load i8, i8* %175, align 1, !tbaa !2
  %335 = xor i8 %334, %333
  store i8 %335, i8* %175, align 1, !tbaa !2
  %336 = load i8, i8* %184, align 1, !tbaa !2
  %337 = zext i8 %336 to i64
  %338 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %337
  %339 = load i8, i8* %338, align 1, !tbaa !2
  %340 = load i8, i8* %185, align 1, !tbaa !2
  %341 = xor i8 %340, %339
  store i8 %341, i8* %185, align 1, !tbaa !2
  %342 = load i8, i8* %186, align 1, !tbaa !2
  %343 = zext i8 %342 to i64
  %344 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %343
  %345 = load i8, i8* %344, align 1, !tbaa !2
  %346 = load i8, i8* %187, align 1, !tbaa !2
  %347 = xor i8 %346, %345
  store i8 %347, i8* %187, align 1, !tbaa !2
  %348 = load i8, i8* %188, align 1, !tbaa !2
  %349 = zext i8 %348 to i64
  %350 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %349
  %351 = load i8, i8* %350, align 1, !tbaa !2
  %352 = load i8, i8* %189, align 1, !tbaa !2
  %353 = xor i8 %352, %351
  store i8 %353, i8* %189, align 1, !tbaa !2
  br label %354

354:                                              ; preds = %354, %328
  %355 = phi i8 [ %335, %328 ], [ %359, %354 ]
  %356 = phi i64 [ 20, %328 ], [ %382, %354 ]
  %357 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %356
  %358 = load i8, i8* %357, align 1, !tbaa !2
  %359 = xor i8 %358, %355
  store i8 %359, i8* %357, align 1, !tbaa !2
  %360 = add nsw i64 %356, -3
  %361 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %360
  %362 = load i8, i8* %361, align 1, !tbaa !2
  %363 = or i64 %356, 1
  %364 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %363
  %365 = load i8, i8* %364, align 1, !tbaa !2
  %366 = xor i8 %365, %362
  store i8 %366, i8* %364, align 1, !tbaa !2
  %367 = add nsw i64 %356, -2
  %368 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %367
  %369 = load i8, i8* %368, align 1, !tbaa !2
  %370 = or i64 %356, 2
  %371 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %370
  %372 = load i8, i8* %371, align 1, !tbaa !2
  %373 = xor i8 %372, %369
  store i8 %373, i8* %371, align 1, !tbaa !2
  %374 = add nsw i64 %356, -1
  %375 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %374
  %376 = load i8, i8* %375, align 1, !tbaa !2
  %377 = or i64 %356, 3
  %378 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %377
  %379 = load i8, i8* %378, align 1, !tbaa !2
  %380 = xor i8 %379, %376
  store i8 %380, i8* %378, align 1, !tbaa !2
  %381 = icmp ult i64 %356, 28
  %382 = add nuw nsw i64 %356, 4
  br i1 %381, label %354, label %383

383:                                              ; preds = %354, %383
  %384 = phi i64 [ %390, %383 ], [ 15, %354 ]
  %385 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %384
  %386 = load i8, i8* %385, align 1, !tbaa !2
  %387 = getelementptr inbounds i8, i8* %2, i64 %384
  %388 = load i8, i8* %387, align 1, !tbaa !2
  %389 = xor i8 %388, %386
  store i8 %389, i8* %387, align 1, !tbaa !2
  %390 = add nsw i64 %384, -1
  %391 = icmp eq i64 %384, 0
  br i1 %391, label %392, label %383

392:                                              ; preds = %383
  %393 = xor i8 %298, %329
  br label %394

394:                                              ; preds = %262, %392
  %395 = phi i8 [ %393, %392 ], [ %192, %262 ]
  %396 = add nuw nsw i8 %191, 1
  %397 = icmp eq i8 %396, 14
  br i1 %397, label %398, label %190

398:                                              ; preds = %394, %398
  %399 = phi i64 [ %405, %398 ], [ 15, %394 ]
  %400 = getelementptr inbounds i8, i8* %2, i64 %399
  %401 = load i8, i8* %400, align 1, !tbaa !2
  %402 = zext i8 %401 to i64
  %403 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %402
  %404 = load i8, i8* %403, align 1, !tbaa !2
  store i8 %404, i8* %400, align 1, !tbaa !2
  %405 = add nsw i64 %399, -1
  %406 = icmp eq i64 %399, 0
  br i1 %406, label %407, label %398

407:                                              ; preds = %398
  %408 = load i8, i8* %163, align 1, !tbaa !2
  %409 = load i8, i8* %164, align 1, !tbaa !2
  store i8 %409, i8* %163, align 1, !tbaa !2
  %410 = load i8, i8* %165, align 1, !tbaa !2
  store i8 %410, i8* %164, align 1, !tbaa !2
  %411 = load i8, i8* %166, align 1, !tbaa !2
  store i8 %411, i8* %165, align 1, !tbaa !2
  store i8 %408, i8* %166, align 1, !tbaa !2
  %412 = load i8, i8* %167, align 1, !tbaa !2
  %413 = load i8, i8* %168, align 1, !tbaa !2
  store i8 %413, i8* %167, align 1, !tbaa !2
  store i8 %412, i8* %168, align 1, !tbaa !2
  %414 = load i8, i8* %169, align 1, !tbaa !2
  %415 = load i8, i8* %170, align 1, !tbaa !2
  store i8 %415, i8* %169, align 1, !tbaa !2
  %416 = load i8, i8* %171, align 1, !tbaa !2
  store i8 %416, i8* %170, align 1, !tbaa !2
  %417 = load i8, i8* %172, align 1, !tbaa !2
  store i8 %417, i8* %171, align 1, !tbaa !2
  store i8 %414, i8* %172, align 1, !tbaa !2
  %418 = load i8, i8* %173, align 1, !tbaa !2
  %419 = load i8, i8* %174, align 1, !tbaa !2
  store i8 %419, i8* %173, align 1, !tbaa !2
  store i8 %418, i8* %174, align 1, !tbaa !2
  %420 = load i8, i8* %176, align 1, !tbaa !2
  %421 = zext i8 %420 to i64
  %422 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %421
  %423 = load i8, i8* %422, align 1, !tbaa !2
  %424 = xor i8 %423, %395
  %425 = load i8, i8* %162, align 1, !tbaa !2
  %426 = xor i8 %424, %425
  store i8 %426, i8* %162, align 1, !tbaa !2
  %427 = load i8, i8* %177, align 1, !tbaa !2
  %428 = zext i8 %427 to i64
  %429 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %428
  %430 = load i8, i8* %429, align 1, !tbaa !2
  %431 = load i8, i8* %178, align 1, !tbaa !2
  %432 = xor i8 %431, %430
  store i8 %432, i8* %178, align 1, !tbaa !2
  %433 = load i8, i8* %179, align 1, !tbaa !2
  %434 = zext i8 %433 to i64
  %435 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %434
  %436 = load i8, i8* %435, align 1, !tbaa !2
  %437 = load i8, i8* %180, align 1, !tbaa !2
  %438 = xor i8 %437, %436
  store i8 %438, i8* %180, align 1, !tbaa !2
  %439 = load i8, i8* %181, align 1, !tbaa !2
  %440 = zext i8 %439 to i64
  %441 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %440
  %442 = load i8, i8* %441, align 1, !tbaa !2
  %443 = load i8, i8* %182, align 1, !tbaa !2
  %444 = xor i8 %443, %442
  store i8 %444, i8* %182, align 1, !tbaa !2
  br label %445

445:                                              ; preds = %445, %407
  %446 = phi i8 [ %426, %407 ], [ %450, %445 ]
  %447 = phi i64 [ 4, %407 ], [ %473, %445 ]
  %448 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %447
  %449 = load i8, i8* %448, align 1, !tbaa !2
  %450 = xor i8 %449, %446
  store i8 %450, i8* %448, align 1, !tbaa !2
  %451 = add nsw i64 %447, -3
  %452 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %451
  %453 = load i8, i8* %452, align 1, !tbaa !2
  %454 = or i64 %447, 1
  %455 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %454
  %456 = load i8, i8* %455, align 1, !tbaa !2
  %457 = xor i8 %456, %453
  store i8 %457, i8* %455, align 1, !tbaa !2
  %458 = add nsw i64 %447, -2
  %459 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %458
  %460 = load i8, i8* %459, align 1, !tbaa !2
  %461 = or i64 %447, 2
  %462 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %461
  %463 = load i8, i8* %462, align 1, !tbaa !2
  %464 = xor i8 %463, %460
  store i8 %464, i8* %462, align 1, !tbaa !2
  %465 = add nsw i64 %447, -1
  %466 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %465
  %467 = load i8, i8* %466, align 1, !tbaa !2
  %468 = or i64 %447, 3
  %469 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %468
  %470 = load i8, i8* %469, align 1, !tbaa !2
  %471 = xor i8 %470, %467
  store i8 %471, i8* %469, align 1, !tbaa !2
  %472 = icmp ult i64 %447, 12
  %473 = add nuw nsw i64 %447, 4
  br i1 %472, label %445, label %474

474:                                              ; preds = %445
  %475 = load i8, i8* %183, align 1, !tbaa !2
  %476 = zext i8 %475 to i64
  %477 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %476
  %478 = load i8, i8* %477, align 1, !tbaa !2
  %479 = load i8, i8* %175, align 1, !tbaa !2
  %480 = xor i8 %479, %478
  store i8 %480, i8* %175, align 1, !tbaa !2
  %481 = load i8, i8* %184, align 1, !tbaa !2
  %482 = zext i8 %481 to i64
  %483 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %482
  %484 = load i8, i8* %483, align 1, !tbaa !2
  %485 = load i8, i8* %185, align 1, !tbaa !2
  %486 = xor i8 %485, %484
  store i8 %486, i8* %185, align 1, !tbaa !2
  %487 = load i8, i8* %186, align 1, !tbaa !2
  %488 = zext i8 %487 to i64
  %489 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %488
  %490 = load i8, i8* %489, align 1, !tbaa !2
  %491 = load i8, i8* %187, align 1, !tbaa !2
  %492 = xor i8 %491, %490
  store i8 %492, i8* %187, align 1, !tbaa !2
  %493 = load i8, i8* %188, align 1, !tbaa !2
  %494 = zext i8 %493 to i64
  %495 = getelementptr inbounds [256 x i8], [256 x i8]* @sbox, i64 0, i64 %494
  %496 = load i8, i8* %495, align 1, !tbaa !2
  %497 = load i8, i8* %189, align 1, !tbaa !2
  %498 = xor i8 %497, %496
  store i8 %498, i8* %189, align 1, !tbaa !2
  br label %499

499:                                              ; preds = %499, %474
  %500 = phi i8 [ %480, %474 ], [ %504, %499 ]
  %501 = phi i64 [ 20, %474 ], [ %527, %499 ]
  %502 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %501
  %503 = load i8, i8* %502, align 1, !tbaa !2
  %504 = xor i8 %503, %500
  store i8 %504, i8* %502, align 1, !tbaa !2
  %505 = add nsw i64 %501, -3
  %506 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %505
  %507 = load i8, i8* %506, align 1, !tbaa !2
  %508 = or i64 %501, 1
  %509 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %508
  %510 = load i8, i8* %509, align 1, !tbaa !2
  %511 = xor i8 %510, %507
  store i8 %511, i8* %509, align 1, !tbaa !2
  %512 = add nsw i64 %501, -2
  %513 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %512
  %514 = load i8, i8* %513, align 1, !tbaa !2
  %515 = or i64 %501, 2
  %516 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %515
  %517 = load i8, i8* %516, align 1, !tbaa !2
  %518 = xor i8 %517, %514
  store i8 %518, i8* %516, align 1, !tbaa !2
  %519 = add nsw i64 %501, -1
  %520 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %519
  %521 = load i8, i8* %520, align 1, !tbaa !2
  %522 = or i64 %501, 3
  %523 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %522
  %524 = load i8, i8* %523, align 1, !tbaa !2
  %525 = xor i8 %524, %521
  store i8 %525, i8* %523, align 1, !tbaa !2
  %526 = icmp ult i64 %501, 28
  %527 = add nuw nsw i64 %501, 4
  br i1 %526, label %499, label %528

528:                                              ; preds = %499, %528
  %529 = phi i64 [ %535, %528 ], [ 15, %499 ]
  %530 = getelementptr inbounds %struct.aes256_context, %struct.aes256_context* %0, i64 0, i32 0, i64 %529
  %531 = load i8, i8* %530, align 1, !tbaa !2
  %532 = getelementptr inbounds i8, i8* %2, i64 %529
  %533 = load i8, i8* %532, align 1, !tbaa !2
  %534 = xor i8 %533, %531
  store i8 %534, i8* %532, align 1, !tbaa !2
  %535 = add nsw i64 %529, -1
  %536 = icmp eq i64 %529, 0
  br i1 %536, label %537, label %528

537:                                              ; preds = %528
  ret void
}

attributes #0 = { alwaysinline norecurse nounwind optsize readnone uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { alwaysinline nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
