; ModuleID = 'nw.c'
source_filename = "nw.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local i32 @needwun(i8* nocapture readonly %0, i8* nocapture readonly %1, i8* nocapture readnone %2, i8* nocapture readnone %3, i32* nocapture %4, i8* nocapture %5) local_unnamed_addr #0 {
  br label %7

7:                                                ; preds = %6, %7
  %8 = phi i64 [ 0, %6 ], [ %12, %7 ]
  %9 = getelementptr inbounds i32, i32* %4, i64 %8
  %10 = trunc i64 %8 to i32
  %11 = sub i32 0, %10
  store i32 %11, i32* %9, align 4, !tbaa !2
  %12 = add nuw nsw i64 %8, 1
  %13 = icmp eq i64 %12, 129
  br i1 %13, label %14, label %7

14:                                               ; preds = %7, %14
  %15 = phi i64 [ %20, %14 ], [ 0, %7 ]
  %16 = mul nuw nsw i64 %15, 129
  %17 = getelementptr inbounds i32, i32* %4, i64 %16
  %18 = trunc i64 %15 to i32
  %19 = sub i32 0, %18
  store i32 %19, i32* %17, align 4, !tbaa !2
  %20 = add nuw nsw i64 %15, 1
  %21 = icmp eq i64 %20, 129
  br i1 %21, label %22, label %14

22:                                               ; preds = %14, %72
  %23 = phi i64 [ %73, %72 ], [ 1, %14 ]
  %24 = phi i32 [ %60, %72 ], [ undef, %14 ]
  %25 = phi i32 [ %30, %72 ], [ undef, %14 ]
  %26 = add nsw i64 %23, -1
  %27 = getelementptr inbounds i8, i8* %1, i64 %26
  %28 = mul nuw nsw i64 %26, 129
  %29 = mul nuw nsw i64 %23, 129
  %30 = trunc i64 %29 to i32
  br label %31

31:                                               ; preds = %22, %69
  %32 = phi i64 [ 1, %22 ], [ %70, %69 ]
  %33 = phi i32 [ %24, %22 ], [ %60, %69 ]
  %34 = phi i32 [ %25, %22 ], [ %30, %69 ]
  %35 = add nsw i64 %32, -1
  %36 = getelementptr inbounds i8, i8* %0, i64 %35
  %37 = load i8, i8* %36, align 1, !tbaa !6
  %38 = load i8, i8* %27, align 1, !tbaa !6
  %39 = icmp eq i8 %37, %38
  %40 = select i1 %39, i32 1, i32 -1
  %41 = add nuw nsw i64 %35, %28
  %42 = getelementptr inbounds i32, i32* %4, i64 %41
  %43 = load i32, i32* %42, align 4, !tbaa !2
  %44 = add nsw i32 %40, %43
  %45 = add nuw nsw i64 %32, %28
  %46 = getelementptr inbounds i32, i32* %4, i64 %45
  %47 = load i32, i32* %46, align 4, !tbaa !2
  %48 = add nsw i32 %47, -1
  %49 = add nuw nsw i64 %35, %29
  %50 = getelementptr inbounds i32, i32* %4, i64 %49
  %51 = load i32, i32* %50, align 4, !tbaa !2
  %52 = add nsw i32 %51, -1
  %53 = icmp sgt i32 %47, %51
  %54 = select i1 %53, i32 %48, i32 %52
  %55 = icmp sgt i32 %44, %54
  %56 = select i1 %55, i32 %44, i32 %54
  %57 = add nuw nsw i64 %32, %29
  %58 = getelementptr inbounds i32, i32* %4, i64 %57
  store i32 %56, i32* %58, align 4, !tbaa !2
  %59 = add i32 %34, 1024
  %60 = add i32 %59, %33
  %61 = icmp eq i32 %56, %52
  br i1 %61, label %62, label %64

62:                                               ; preds = %31
  %63 = getelementptr inbounds i8, i8* %5, i64 %57
  store i8 60, i8* %63, align 1, !tbaa !6
  br label %69

64:                                               ; preds = %31
  %65 = icmp eq i32 %56, %48
  %66 = getelementptr inbounds i8, i8* %5, i64 %57
  br i1 %65, label %67, label %68

67:                                               ; preds = %64
  store i8 94, i8* %66, align 1, !tbaa !6
  br label %69

68:                                               ; preds = %64
  store i8 92, i8* %66, align 1, !tbaa !6
  br label %69

69:                                               ; preds = %62, %68, %67
  %70 = add nuw nsw i64 %32, 1
  %71 = icmp eq i64 %70, 129
  br i1 %71, label %72, label %31

72:                                               ; preds = %69
  %73 = add nuw nsw i64 %23, 1
  %74 = icmp eq i64 %73, 129
  br i1 %74, label %75, label %22

75:                                               ; preds = %72
  ret i32 %60
}

attributes #0 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!4, !4, i64 0}
