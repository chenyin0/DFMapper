; ModuleID = 'bfs.c'
source_filename = "bfs.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.node_t_struct = type { i64, i64 }
%struct.edge_t_struct = type { i64 }

; Function Attrs: nounwind optsize uwtable
define dso_local void @bfs(%struct.node_t_struct* nocapture readonly %0, %struct.edge_t_struct* nocapture readonly %1, i64 %2, i8* nocapture %3, i64* nocapture %4) local_unnamed_addr #0 {
  %6 = alloca [256 x i64], align 16
  %7 = bitcast [256 x i64]* %6 to i8*
  call void @llvm.lifetime.start.p0i8(i64 2048, i8* nonnull %7) #2
  %8 = getelementptr inbounds i8, i8* %3, i64 %2
  store i8 0, i8* %8, align 1, !tbaa !2
  store i64 1, i64* %4, align 8, !tbaa !5
  %9 = getelementptr inbounds [256 x i64], [256 x i64]* %6, i64 0, i64 0
  store i64 %2, i64* %9, align 16, !tbaa !5
  br label %10

10:                                               ; preds = %5, %61
  %11 = phi i64 [ 2, %5 ], [ %62, %61 ]
  %12 = phi i64 [ 0, %5 ], [ %28, %61 ]
  %13 = phi i64 [ 0, %5 ], [ %63, %61 ]
  %14 = icmp ugt i64 %11, %12
  br i1 %14, label %15, label %18

15:                                               ; preds = %10
  %16 = add nuw nsw i64 %12, 1
  %17 = icmp eq i64 %11, %16
  br i1 %17, label %65, label %24

18:                                               ; preds = %10
  %19 = icmp eq i64 %11, 0
  %20 = icmp eq i64 %12, 255
  %21 = and i1 %20, %19
  br i1 %21, label %65, label %22

22:                                               ; preds = %18
  %23 = add nuw nsw i64 %12, 1
  br label %24

24:                                               ; preds = %22, %15
  %25 = phi i64 [ %23, %22 ], [ %16, %15 ]
  %26 = getelementptr inbounds [256 x i64], [256 x i64]* %6, i64 0, i64 %12
  %27 = load i64, i64* %26, align 8, !tbaa !5
  %28 = and i64 %25, 255
  %29 = getelementptr inbounds %struct.node_t_struct, %struct.node_t_struct* %0, i64 %27, i32 0
  %30 = load i64, i64* %29, align 8, !tbaa !7
  %31 = getelementptr inbounds %struct.node_t_struct, %struct.node_t_struct* %0, i64 %27, i32 1
  %32 = load i64, i64* %31, align 8, !tbaa !9
  %33 = icmp ult i64 %30, %32
  br i1 %33, label %34, label %61

34:                                               ; preds = %24
  %35 = getelementptr inbounds i8, i8* %3, i64 %27
  br label %36

36:                                               ; preds = %34, %57
  %37 = phi i64 [ %11, %34 ], [ %58, %57 ]
  %38 = phi i64 [ %30, %34 ], [ %59, %57 ]
  %39 = getelementptr inbounds %struct.edge_t_struct, %struct.edge_t_struct* %1, i64 %38, i32 0
  %40 = load i64, i64* %39, align 8, !tbaa !10
  %41 = getelementptr inbounds i8, i8* %3, i64 %40
  %42 = load i8, i8* %41, align 1, !tbaa !2
  %43 = icmp eq i8 %42, 127
  br i1 %43, label %44, label %57

44:                                               ; preds = %36
  %45 = load i8, i8* %35, align 1, !tbaa !2
  %46 = add i8 %45, 1
  store i8 %46, i8* %41, align 1, !tbaa !2
  %47 = sext i8 %46 to i64
  %48 = getelementptr inbounds i64, i64* %4, i64 %47
  %49 = load i64, i64* %48, align 8, !tbaa !5
  %50 = add i64 %49, 1
  store i64 %50, i64* %48, align 8, !tbaa !5
  %51 = icmp eq i64 %37, 0
  %52 = add i64 %37, -1
  %53 = select i1 %51, i64 255, i64 %52
  %54 = getelementptr inbounds [256 x i64], [256 x i64]* %6, i64 0, i64 %53
  store i64 %40, i64* %54, align 8, !tbaa !5
  %55 = add i64 %37, 1
  %56 = and i64 %55, 255
  br label %57

57:                                               ; preds = %44, %36
  %58 = phi i64 [ %56, %44 ], [ %37, %36 ]
  %59 = add nuw i64 %38, 1
  %60 = icmp eq i64 %59, %32
  br i1 %60, label %61, label %36

61:                                               ; preds = %57, %24
  %62 = phi i64 [ %11, %24 ], [ %58, %57 ]
  %63 = add nuw nsw i64 %13, 1
  %64 = icmp eq i64 %63, 256
  br i1 %64, label %65, label %10

65:                                               ; preds = %15, %18, %61
  call void @llvm.lifetime.end.p0i8(i64 2048, i8* nonnull %7) #2
  ret void
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
!6 = !{!"long", !3, i64 0}
!7 = !{!8, !6, i64 0}
!8 = !{!"node_t_struct", !6, i64 0, !6, i64 8}
!9 = !{!8, !6, i64 8}
!10 = !{!11, !6, i64 0}
!11 = !{!"edge_t_struct", !6, i64 0}
