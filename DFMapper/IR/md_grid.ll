; ModuleID = 'md.c'
source_filename = "md.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.dvector_t = type { double, double, double }
%struct.ivector_t = type { i32, i32, i32 }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @md([4 x [4 x i32]]* %0, [4 x [4 x [10 x %struct.dvector_t]]]* %1, [4 x [4 x [10 x %struct.dvector_t]]]* %2) #0 {
  %4 = alloca [4 x [4 x i32]]*, align 8
  %5 = alloca [4 x [4 x [10 x %struct.dvector_t]]]*, align 8
  %6 = alloca [4 x [4 x [10 x %struct.dvector_t]]]*, align 8
  %7 = alloca %struct.ivector_t, align 4
  %8 = alloca %struct.ivector_t, align 4
  %9 = alloca %struct.dvector_t, align 8
  %10 = alloca %struct.dvector_t, align 8
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca double, align 8
  %14 = alloca double, align 8
  %15 = alloca double, align 8
  %16 = alloca double, align 8
  %17 = alloca double, align 8
  %18 = alloca double, align 8
  %19 = alloca double, align 8
  %20 = alloca %struct.dvector_t*, align 8
  %21 = alloca i32, align 4
  %22 = alloca double, align 8
  %23 = alloca double, align 8
  %24 = alloca double, align 8
  store [4 x [4 x i32]]* %0, [4 x [4 x i32]]** %4, align 8
  store [4 x [4 x [10 x %struct.dvector_t]]]* %1, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  store [4 x [4 x [10 x %struct.dvector_t]]]* %2, [4 x [4 x [10 x %struct.dvector_t]]]** %6, align 8
  br label %25

25:                                               ; preds = %3
  %26 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  store i32 0, i32* %26, align 4
  br label %27

27:                                               ; preds = %425, %25
  %28 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %29 = load i32, i32* %28, align 4
  %30 = icmp slt i32 %29, 4
  br i1 %30, label %31, label %429

31:                                               ; preds = %27
  br label %32

32:                                               ; preds = %31
  %33 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  store i32 0, i32* %33, align 4
  br label %34

34:                                               ; preds = %420, %32
  %35 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %36 = load i32, i32* %35, align 4
  %37 = icmp slt i32 %36, 4
  br i1 %37, label %38, label %424

38:                                               ; preds = %34
  br label %39

39:                                               ; preds = %38
  %40 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  store i32 0, i32* %40, align 4
  br label %41

41:                                               ; preds = %415, %39
  %42 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %43 = load i32, i32* %42, align 4
  %44 = icmp slt i32 %43, 4
  br i1 %44, label %45, label %419

45:                                               ; preds = %41
  br label %46

46:                                               ; preds = %45
  %47 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %48 = load i32, i32* %47, align 4
  %49 = sub nsw i32 %48, 1
  %50 = icmp sgt i32 0, %49
  br i1 %50, label %51, label %52

51:                                               ; preds = %46
  br label %56

52:                                               ; preds = %46
  %53 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %54 = load i32, i32* %53, align 4
  %55 = sub nsw i32 %54, 1
  br label %56

56:                                               ; preds = %52, %51
  %57 = phi i32 [ 0, %51 ], [ %55, %52 ]
  %58 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 0
  store i32 %57, i32* %58, align 4
  br label %59

59:                                               ; preds = %410, %56
  %60 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 0
  %61 = load i32, i32* %60, align 4
  %62 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %63 = load i32, i32* %62, align 4
  %64 = add nsw i32 %63, 2
  %65 = icmp slt i32 4, %64
  br i1 %65, label %66, label %67

66:                                               ; preds = %59
  br label %71

67:                                               ; preds = %59
  %68 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %69 = load i32, i32* %68, align 4
  %70 = add nsw i32 %69, 2
  br label %71

71:                                               ; preds = %67, %66
  %72 = phi i32 [ 4, %66 ], [ %70, %67 ]
  %73 = icmp slt i32 %61, %72
  br i1 %73, label %74, label %414

74:                                               ; preds = %71
  br label %75

75:                                               ; preds = %74
  %76 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %77 = load i32, i32* %76, align 4
  %78 = sub nsw i32 %77, 1
  %79 = icmp sgt i32 0, %78
  br i1 %79, label %80, label %81

80:                                               ; preds = %75
  br label %85

81:                                               ; preds = %75
  %82 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %83 = load i32, i32* %82, align 4
  %84 = sub nsw i32 %83, 1
  br label %85

85:                                               ; preds = %81, %80
  %86 = phi i32 [ 0, %80 ], [ %84, %81 ]
  %87 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 1
  store i32 %86, i32* %87, align 4
  br label %88

88:                                               ; preds = %405, %85
  %89 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 1
  %90 = load i32, i32* %89, align 4
  %91 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %92 = load i32, i32* %91, align 4
  %93 = add nsw i32 %92, 2
  %94 = icmp slt i32 4, %93
  br i1 %94, label %95, label %96

95:                                               ; preds = %88
  br label %100

96:                                               ; preds = %88
  %97 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %98 = load i32, i32* %97, align 4
  %99 = add nsw i32 %98, 2
  br label %100

100:                                              ; preds = %96, %95
  %101 = phi i32 [ 4, %95 ], [ %99, %96 ]
  %102 = icmp slt i32 %90, %101
  br i1 %102, label %103, label %409

103:                                              ; preds = %100
  br label %104

104:                                              ; preds = %103
  %105 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %106 = load i32, i32* %105, align 4
  %107 = sub nsw i32 %106, 1
  %108 = icmp sgt i32 0, %107
  br i1 %108, label %109, label %110

109:                                              ; preds = %104
  br label %114

110:                                              ; preds = %104
  %111 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %112 = load i32, i32* %111, align 4
  %113 = sub nsw i32 %112, 1
  br label %114

114:                                              ; preds = %110, %109
  %115 = phi i32 [ 0, %109 ], [ %113, %110 ]
  %116 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 2
  store i32 %115, i32* %116, align 4
  br label %117

117:                                              ; preds = %400, %114
  %118 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 2
  %119 = load i32, i32* %118, align 4
  %120 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %121 = load i32, i32* %120, align 4
  %122 = add nsw i32 %121, 2
  %123 = icmp slt i32 4, %122
  br i1 %123, label %124, label %125

124:                                              ; preds = %117
  br label %129

125:                                              ; preds = %117
  %126 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %127 = load i32, i32* %126, align 4
  %128 = add nsw i32 %127, 2
  br label %129

129:                                              ; preds = %125, %124
  %130 = phi i32 [ 4, %124 ], [ %128, %125 ]
  %131 = icmp slt i32 %119, %130
  br i1 %131, label %132, label %404

132:                                              ; preds = %129
  %133 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %6, align 8
  %134 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 0
  %135 = load i32, i32* %134, align 4
  %136 = sext i32 %135 to i64
  %137 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %133, i64 %136
  %138 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 1
  %139 = load i32, i32* %138, align 4
  %140 = sext i32 %139 to i64
  %141 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %137, i64 0, i64 %140
  %142 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 2
  %143 = load i32, i32* %142, align 4
  %144 = sext i32 %143 to i64
  %145 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %141, i64 0, i64 %144
  %146 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %145, i64 0, i64 0
  store %struct.dvector_t* %146, %struct.dvector_t** %20, align 8
  %147 = load [4 x [4 x i32]]*, [4 x [4 x i32]]** %4, align 8
  %148 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 0
  %149 = load i32, i32* %148, align 4
  %150 = sext i32 %149 to i64
  %151 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %147, i64 %150
  %152 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 1
  %153 = load i32, i32* %152, align 4
  %154 = sext i32 %153 to i64
  %155 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %151, i64 0, i64 %154
  %156 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 2
  %157 = load i32, i32* %156, align 4
  %158 = sext i32 %157 to i64
  %159 = getelementptr inbounds [4 x i32], [4 x i32]* %155, i64 0, i64 %158
  %160 = load i32, i32* %159, align 4
  store i32 %160, i32* %21, align 4
  br label %161

161:                                              ; preds = %132
  store i32 0, i32* %11, align 4
  br label %162

162:                                              ; preds = %396, %161
  %163 = load i32, i32* %11, align 4
  %164 = load [4 x [4 x i32]]*, [4 x [4 x i32]]** %4, align 8
  %165 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %166 = load i32, i32* %165, align 4
  %167 = sext i32 %166 to i64
  %168 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %164, i64 %167
  %169 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %170 = load i32, i32* %169, align 4
  %171 = sext i32 %170 to i64
  %172 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %168, i64 0, i64 %171
  %173 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %174 = load i32, i32* %173, align 4
  %175 = sext i32 %174 to i64
  %176 = getelementptr inbounds [4 x i32], [4 x i32]* %172, i64 0, i64 %175
  %177 = load i32, i32* %176, align 4
  %178 = icmp slt i32 %163, %177
  br i1 %178, label %179, label %399

179:                                              ; preds = %162
  %180 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %6, align 8
  %181 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %182 = load i32, i32* %181, align 4
  %183 = sext i32 %182 to i64
  %184 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %180, i64 %183
  %185 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %186 = load i32, i32* %185, align 4
  %187 = sext i32 %186 to i64
  %188 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %184, i64 0, i64 %187
  %189 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %190 = load i32, i32* %189, align 4
  %191 = sext i32 %190 to i64
  %192 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %188, i64 0, i64 %191
  %193 = load i32, i32* %11, align 4
  %194 = sext i32 %193 to i64
  %195 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %192, i64 0, i64 %194
  %196 = bitcast %struct.dvector_t* %9 to i8*
  %197 = bitcast %struct.dvector_t* %195 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %196, i8* align 8 %197, i64 24, i1 false)
  %198 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  %199 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %200 = load i32, i32* %199, align 4
  %201 = sext i32 %200 to i64
  %202 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %198, i64 %201
  %203 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %204 = load i32, i32* %203, align 4
  %205 = sext i32 %204 to i64
  %206 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %202, i64 0, i64 %205
  %207 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %208 = load i32, i32* %207, align 4
  %209 = sext i32 %208 to i64
  %210 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %206, i64 0, i64 %209
  %211 = load i32, i32* %11, align 4
  %212 = sext i32 %211 to i64
  %213 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %210, i64 0, i64 %212
  %214 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %213, i32 0, i32 0
  %215 = load double, double* %214, align 8
  store double %215, double* %22, align 8
  %216 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  %217 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %218 = load i32, i32* %217, align 4
  %219 = sext i32 %218 to i64
  %220 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %216, i64 %219
  %221 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %222 = load i32, i32* %221, align 4
  %223 = sext i32 %222 to i64
  %224 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %220, i64 0, i64 %223
  %225 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %226 = load i32, i32* %225, align 4
  %227 = sext i32 %226 to i64
  %228 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %224, i64 0, i64 %227
  %229 = load i32, i32* %11, align 4
  %230 = sext i32 %229 to i64
  %231 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %228, i64 0, i64 %230
  %232 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %231, i32 0, i32 1
  %233 = load double, double* %232, align 8
  store double %233, double* %23, align 8
  %234 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  %235 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %236 = load i32, i32* %235, align 4
  %237 = sext i32 %236 to i64
  %238 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %234, i64 %237
  %239 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %240 = load i32, i32* %239, align 4
  %241 = sext i32 %240 to i64
  %242 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %238, i64 0, i64 %241
  %243 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %244 = load i32, i32* %243, align 4
  %245 = sext i32 %244 to i64
  %246 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %242, i64 0, i64 %245
  %247 = load i32, i32* %11, align 4
  %248 = sext i32 %247 to i64
  %249 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %246, i64 0, i64 %248
  %250 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %249, i32 0, i32 2
  %251 = load double, double* %250, align 8
  store double %251, double* %24, align 8
  br label %252

252:                                              ; preds = %179
  store i32 0, i32* %12, align 4
  br label %253

253:                                              ; preds = %338, %252
  %254 = load i32, i32* %12, align 4
  %255 = load i32, i32* %21, align 4
  %256 = icmp slt i32 %254, %255
  br i1 %256, label %257, label %341

257:                                              ; preds = %253
  %258 = load %struct.dvector_t*, %struct.dvector_t** %20, align 8
  %259 = load i32, i32* %12, align 4
  %260 = sext i32 %259 to i64
  %261 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %258, i64 %260
  %262 = bitcast %struct.dvector_t* %10 to i8*
  %263 = bitcast %struct.dvector_t* %261 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %262, i8* align 8 %263, i64 24, i1 false)
  %264 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %10, i32 0, i32 0
  %265 = load double, double* %264, align 8
  %266 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %9, i32 0, i32 0
  %267 = load double, double* %266, align 8
  %268 = fcmp une double %265, %267
  br i1 %268, label %281, label %269

269:                                              ; preds = %257
  %270 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %10, i32 0, i32 1
  %271 = load double, double* %270, align 8
  %272 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %9, i32 0, i32 1
  %273 = load double, double* %272, align 8
  %274 = fcmp une double %271, %273
  br i1 %274, label %281, label %275

275:                                              ; preds = %269
  %276 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %10, i32 0, i32 2
  %277 = load double, double* %276, align 8
  %278 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %9, i32 0, i32 2
  %279 = load double, double* %278, align 8
  %280 = fcmp une double %277, %279
  br i1 %280, label %281, label %337

281:                                              ; preds = %275, %269, %257
  %282 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %9, i32 0, i32 0
  %283 = load double, double* %282, align 8
  %284 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %10, i32 0, i32 0
  %285 = load double, double* %284, align 8
  %286 = fsub double %283, %285
  store double %286, double* %13, align 8
  %287 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %9, i32 0, i32 1
  %288 = load double, double* %287, align 8
  %289 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %10, i32 0, i32 1
  %290 = load double, double* %289, align 8
  %291 = fsub double %288, %290
  store double %291, double* %14, align 8
  %292 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %9, i32 0, i32 2
  %293 = load double, double* %292, align 8
  %294 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %10, i32 0, i32 2
  %295 = load double, double* %294, align 8
  %296 = fsub double %293, %295
  store double %296, double* %15, align 8
  %297 = load double, double* %13, align 8
  %298 = load double, double* %13, align 8
  %299 = fmul double %297, %298
  %300 = load double, double* %14, align 8
  %301 = load double, double* %14, align 8
  %302 = fmul double %300, %301
  %303 = fadd double %299, %302
  %304 = load double, double* %15, align 8
  %305 = load double, double* %15, align 8
  %306 = fmul double %304, %305
  %307 = fadd double %303, %306
  %308 = fdiv double 1.000000e+00, %307
  store double %308, double* %16, align 8
  %309 = load double, double* %16, align 8
  %310 = load double, double* %16, align 8
  %311 = fmul double %309, %310
  %312 = load double, double* %16, align 8
  %313 = fmul double %311, %312
  store double %313, double* %17, align 8
  %314 = load double, double* %17, align 8
  %315 = load double, double* %17, align 8
  %316 = fmul double 1.500000e+00, %315
  %317 = fsub double %316, 2.000000e+00
  %318 = fmul double %314, %317
  store double %318, double* %18, align 8
  %319 = load double, double* %16, align 8
  %320 = load double, double* %18, align 8
  %321 = fmul double %319, %320
  store double %321, double* %19, align 8
  %322 = load double, double* %19, align 8
  %323 = load double, double* %13, align 8
  %324 = fmul double %322, %323
  %325 = load double, double* %22, align 8
  %326 = fadd double %325, %324
  store double %326, double* %22, align 8
  %327 = load double, double* %19, align 8
  %328 = load double, double* %14, align 8
  %329 = fmul double %327, %328
  %330 = load double, double* %23, align 8
  %331 = fadd double %330, %329
  store double %331, double* %23, align 8
  %332 = load double, double* %19, align 8
  %333 = load double, double* %15, align 8
  %334 = fmul double %332, %333
  %335 = load double, double* %24, align 8
  %336 = fadd double %335, %334
  store double %336, double* %24, align 8
  br label %337

337:                                              ; preds = %281, %275
  br label %338

338:                                              ; preds = %337
  %339 = load i32, i32* %12, align 4
  %340 = add nsw i32 %339, 1
  store i32 %340, i32* %12, align 4
  br label %253

341:                                              ; preds = %253
  %342 = load double, double* %22, align 8
  %343 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  %344 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %345 = load i32, i32* %344, align 4
  %346 = sext i32 %345 to i64
  %347 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %343, i64 %346
  %348 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %349 = load i32, i32* %348, align 4
  %350 = sext i32 %349 to i64
  %351 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %347, i64 0, i64 %350
  %352 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %353 = load i32, i32* %352, align 4
  %354 = sext i32 %353 to i64
  %355 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %351, i64 0, i64 %354
  %356 = load i32, i32* %11, align 4
  %357 = sext i32 %356 to i64
  %358 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %355, i64 0, i64 %357
  %359 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %358, i32 0, i32 0
  store double %342, double* %359, align 8
  %360 = load double, double* %23, align 8
  %361 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  %362 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %363 = load i32, i32* %362, align 4
  %364 = sext i32 %363 to i64
  %365 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %361, i64 %364
  %366 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %367 = load i32, i32* %366, align 4
  %368 = sext i32 %367 to i64
  %369 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %365, i64 0, i64 %368
  %370 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %371 = load i32, i32* %370, align 4
  %372 = sext i32 %371 to i64
  %373 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %369, i64 0, i64 %372
  %374 = load i32, i32* %11, align 4
  %375 = sext i32 %374 to i64
  %376 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %373, i64 0, i64 %375
  %377 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %376, i32 0, i32 1
  store double %360, double* %377, align 8
  %378 = load double, double* %24, align 8
  %379 = load [4 x [4 x [10 x %struct.dvector_t]]]*, [4 x [4 x [10 x %struct.dvector_t]]]** %5, align 8
  %380 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %381 = load i32, i32* %380, align 4
  %382 = sext i32 %381 to i64
  %383 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %379, i64 %382
  %384 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %385 = load i32, i32* %384, align 4
  %386 = sext i32 %385 to i64
  %387 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]], [4 x [4 x [10 x %struct.dvector_t]]]* %383, i64 0, i64 %386
  %388 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %389 = load i32, i32* %388, align 4
  %390 = sext i32 %389 to i64
  %391 = getelementptr inbounds [4 x [10 x %struct.dvector_t]], [4 x [10 x %struct.dvector_t]]* %387, i64 0, i64 %390
  %392 = load i32, i32* %11, align 4
  %393 = sext i32 %392 to i64
  %394 = getelementptr inbounds [10 x %struct.dvector_t], [10 x %struct.dvector_t]* %391, i64 0, i64 %393
  %395 = getelementptr inbounds %struct.dvector_t, %struct.dvector_t* %394, i32 0, i32 2
  store double %378, double* %395, align 8
  br label %396

396:                                              ; preds = %341
  %397 = load i32, i32* %11, align 4
  %398 = add nsw i32 %397, 1
  store i32 %398, i32* %11, align 4
  br label %162

399:                                              ; preds = %162
  br label %400

400:                                              ; preds = %399
  %401 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 2
  %402 = load i32, i32* %401, align 4
  %403 = add nsw i32 %402, 1
  store i32 %403, i32* %401, align 4
  br label %117

404:                                              ; preds = %129
  br label %405

405:                                              ; preds = %404
  %406 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 1
  %407 = load i32, i32* %406, align 4
  %408 = add nsw i32 %407, 1
  store i32 %408, i32* %406, align 4
  br label %88

409:                                              ; preds = %100
  br label %410

410:                                              ; preds = %409
  %411 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %8, i32 0, i32 0
  %412 = load i32, i32* %411, align 4
  %413 = add nsw i32 %412, 1
  store i32 %413, i32* %411, align 4
  br label %59

414:                                              ; preds = %71
  br label %415

415:                                              ; preds = %414
  %416 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 2
  %417 = load i32, i32* %416, align 4
  %418 = add nsw i32 %417, 1
  store i32 %418, i32* %416, align 4
  br label %41

419:                                              ; preds = %41
  br label %420

420:                                              ; preds = %419
  %421 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 1
  %422 = load i32, i32* %421, align 4
  %423 = add nsw i32 %422, 1
  store i32 %423, i32* %421, align 4
  br label %34

424:                                              ; preds = %34
  br label %425

425:                                              ; preds = %424
  %426 = getelementptr inbounds %struct.ivector_t, %struct.ivector_t* %7, i32 0, i32 0
  %427 = load i32, i32* %426, align 4
  %428 = add nsw i32 %427, 1
  store i32 %428, i32* %426, align 4
  br label %27

429:                                              ; preds = %27
  ret void
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b4ffc40d622bb2647852284c78c5e83346f2d630)"}
