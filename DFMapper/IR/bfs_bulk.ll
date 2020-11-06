; ModuleID = 'bfs.c'
source_filename = "bfs.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.node_t_struct = type { i64, i64 }
%struct.edge_t_struct = type { i64 }

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @bfs(%struct.node_t_struct* nocapture readonly %0, %struct.edge_t_struct* nocapture readonly %1, i64 %2, i8* nocapture %3, i64* nocapture %4) local_unnamed_addr #0 {
  %6 = getelementptr inbounds i8, i8* %3, i64 %2
  store i8 0, i8* %6, align 1, !tbaa !2
  store i64 1, i64* %4, align 8, !tbaa !5
  br label %7

7:                                                ; preds = %42, %5
  %8 = phi i64 [ 0, %5 ], [ %9, %42 ]
  %9 = add nuw nsw i64 %8, 1
  %10 = trunc i64 %9 to i8
  br label %11

11:                                               ; preds = %7, %38
  %12 = phi i64 [ 0, %7 ], [ %40, %38 ]
  %13 = phi i64 [ 0, %7 ], [ %39, %38 ]
  %14 = getelementptr inbounds i8, i8* %3, i64 %12
  %15 = load i8, i8* %14, align 1, !tbaa !2
  %16 = zext i8 %15 to i64
  %17 = icmp eq i64 %8, %16
  br i1 %17, label %18, label %38

18:                                               ; preds = %11
  %19 = getelementptr inbounds %struct.node_t_struct, %struct.node_t_struct* %0, i64 %12, i32 0
  %20 = load i64, i64* %19, align 8, !tbaa !7
  %21 = getelementptr inbounds %struct.node_t_struct, %struct.node_t_struct* %0, i64 %12, i32 1
  %22 = load i64, i64* %21, align 8, !tbaa !9
  %23 = icmp ult i64 %20, %22
  br i1 %23, label %24, label %38

24:                                               ; preds = %18, %34
  %25 = phi i64 [ %36, %34 ], [ %20, %18 ]
  %26 = phi i64 [ %35, %34 ], [ %13, %18 ]
  %27 = getelementptr inbounds %struct.edge_t_struct, %struct.edge_t_struct* %1, i64 %25, i32 0
  %28 = load i64, i64* %27, align 8, !tbaa !10
  %29 = getelementptr inbounds i8, i8* %3, i64 %28
  %30 = load i8, i8* %29, align 1, !tbaa !2
  %31 = icmp eq i8 %30, 127
  br i1 %31, label %32, label %34

32:                                               ; preds = %24
  store i8 %10, i8* %29, align 1, !tbaa !2
  %33 = add i64 %26, 1
  br label %34

34:                                               ; preds = %32, %24
  %35 = phi i64 [ %33, %32 ], [ %26, %24 ]
  %36 = add nuw i64 %25, 1
  %37 = icmp eq i64 %36, %22
  br i1 %37, label %38, label %24

38:                                               ; preds = %34, %18, %11
  %39 = phi i64 [ %13, %11 ], [ %13, %18 ], [ %35, %34 ]
  %40 = add nuw nsw i64 %12, 1
  %41 = icmp eq i64 %40, 256
  br i1 %41, label %42, label %11

42:                                               ; preds = %38
  %43 = getelementptr inbounds i64, i64* %4, i64 %9
  store i64 %39, i64* %43, align 8, !tbaa !5
  %44 = icmp eq i64 %39, 0
  %45 = icmp eq i64 %9, 10
  %46 = or i1 %44, %45
  br i1 %46, label %47, label %7

47:                                               ; preds = %42
  ret void
}

attributes #0 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !3, i64 0}
!7 = !{!8, !6, i64 0}
!8 = !{!"node_t_struct", !6, i64 0, !6, i64 8}
!9 = !{!8, !6, i64 8}
!10 = !{!11, !6, i64 0}
!11 = !{!"edge_t_struct", !6, i64 0}
