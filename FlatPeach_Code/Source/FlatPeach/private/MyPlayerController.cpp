#include "MyPlayerController.h"
#include "TimerManager.h"
#include "FlatPeachGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Image.h"
#include "WidgetLayoutLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Tree.h"
#include "Vine.h"
#include "Cluster.h"
#include "Cube.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "LogTest.h"
#include "String.h"
#include "Cylinder.h"
#include "Kismet/KismetMathLibrary.h"
#include "Eagle.h"
#include "Warrok.h"
#include "Robot.h"
#include "RobotAIController.h"
#include "Engine.h"


AMyPlayerController::AMyPlayerController() {

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	LastEnergyProduceTime = 0;
	Energy = 1000;
	BDestroy = true;
	BReleasePlanter = false;
	BClickPlanter = false;
	PlanterNum = 5;
	index1 = 99;
	index2 = 99;
	LocToBP = FVector(0,0,0);
	BUseShovel = false;
	BPlant = false;
	GameTime = 0;
}

// Called every frame
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BPauseClick)
	{
		GameTime += DeltaTime;
		//TRACE("index2   %d", index2);
		/*GameTime = GetGameTimeSinceCreation();
		if (i % 2 == 0)
			GameTime = DeltaTime*i;
		i++;*/
	}

	if (GameTime < 60)
	{
		if (GameTime - LastEnergyProduceTime >= 3)
		{
			Energy += 20;
			LastEnergyProduceTime = GameTime;
			CreateAnimals();
		}
	}
	else if (GameTime < 121 && GameTime >= 60)
	{
		if (GameTime - LastEnergyProduceTime >= 2)
		{
			Energy += 20;
			LastEnergyProduceTime = GameTime;
			CreateAnimals();
		}
	}
	else
	{
		if (GameTime - LastEnergyProduceTime >= 1)
		{
			Energy += 20;
			LastEnergyProduceTime = GameTime;
			CreateAnimals();
		}
	}
	PictureFollowMouse(BClickPlanter, PlanterPicture);
	PlantAndDestroy(BReleasePlanter,PlanterNum, EnergyNotEnough);
}

void AMyPlayerController::DestroyEnergyWidget()
{
	BDestroy = true;		//已被销毁
	EnergyNotEnough->RemoveFromParent();
}

void AMyPlayerController::ShovelsFun()
{
	//TRACE("Shovelsfun");
	if (TickTime < 9)  //第三次tick找到鼠标位置才有效，所以设置ticktime计次，我也不知道为啥
	{
		//TRACE("TickTime %d",TickTime);
		if (FindMousePosition() != FVector(0, 0, 0) && FindMousePosition().X >= -735 && FindMousePosition().X <= 850 && FindMousePosition().Y >= -2150 && FindMousePosition().Y <= 530)	//根据返回值判断是否成功种植  850，-735      -2150  530
		{
			//TRACE("mouse pos can find.  %d",TickTime);
			if (TickTime > 8)
			{
				PlanterNum = 5;
				TickTime = 0;
				//TRACE("overtime");
			}
			else
			{
				FVector pos = FindMousePosition();
				FindCubeAndPlanter(pos);//找到cube，然后根据有没有被占领找植物,
			}
		}
	}
	else
	{
		//TRACE("TickTime %d",TickTime);
		TickTime = 0;
		PlanterNum = 5;
	}
}

void AMyPlayerController::FindCubeAndPlanter(FVector pos)
{
	float Length = 10000, Vectorlen;
	int i;
	static int rec;
	FVector actorloc, vec;

	TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACube::StaticClass(), arr);

	for (i = 0; i < 40; i++) //找到最近位置的Cube，记录index
	{
		//TRACE("    %d", i);
		ACube *Cube = Cast<ACube>(arr[i]);
		actorloc = Cube->GetActorLocation();//Cube位置
		vec = actorloc - pos;		//两位置求向量
		Vectorlen = vec.Size();		//向量长度
		if (Vectorlen<Length)
		{
			Length = Vectorlen;
			//TRACE("%d", i)
			//TRACE("length:%f", Length);
			rec = i;
		}
	}
	ACube *Cube = Cast<ACube>(arr[rec]);
	FVector pos1 = Cube->GetActorLocation();
	if (Cube->Occupied)				//如果被占领
	{
		BUseShovel = true;
		index2 = rec;	//利用蓝图解除占用
		//找到那个要消除的植物并进行消除
		ShovelDestroyPlanter(pos1);
		//TRACE("index    %d", rec);
		FString name = Cube->GetName();
		//TRACE("name   %s", *name);
	}
}

void AMyPlayerController::ShovelDestroyPlanter(FVector pos)
{
	//TRACE("pos    %f", pos.X);
	//TRACE("pos    %f", pos.Y);
	//TRACE("pos    %f", pos.Z);

	float a = 20.f, Vectorlen;  //a作为最小判断距离，距离比这还小的就是要移除的对象
	FVector actorloc, vec;

	//找到最近位置的Tree植物，移除
	//for (TActorIterator<ATree*>Itr(GetWorld()); Itr; ++Itr)
	//{
	//	//int32 d = *Itr.GetProgressNumerator;
	//	//TRACE("%d", d);
	//	ATree* Tree = Cast<ATree>(*Itr);
	//	actorloc = Tree->GetActorLocation();
	//	vec = actorloc - pos;
	//	Vectorlen = vec.Size();
	//	if (Vectorlen<a)
	//	{
	//		//移除植物动作
	//		//TreeArr.remove
	//		//Destroy(*Itr);
	//		/*TRACE("      %f", Vectorlen);
	//		int32 h = *Itr.GetProgressNumerator;
	//		TRACE("%d", h);
	//		return;*/
	//	}
	//}
	TArray<AActor*> TreeArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATree::StaticClass(), TreeArr);
	int treenum = TreeArr.Num();
	//TRACE("Treenum   %d", treenum);
	for (int i = 0; i < treenum; i++)
	{
		ATree *Tree = Cast<ATree>(TreeArr[i]);
		actorloc = Tree->GetActorLocation();
		//TRACE("actorlocation       %d", i);
		vec = actorloc - pos;
		Vectorlen = vec.Size();
		if (Vectorlen<a)
		{
			//移除植物动作
			TreeArr[i]->Destroy();
			//TreeArr.RemoveAt(i);
			//TreeArr[i]->RemoveFromRoot;
			//Destroy(TreeArr[i]);
			//TRACE("num   %d", i);
			FString na = Tree->GetName();
			//TRACE("name  %s", *na);
			return;
		}
	}

	//找到最近位置的Vine植物，移除
	TArray<AActor*> VineArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVine::StaticClass(), VineArr);
	
	for (int i = 0; i < VineArr.Num(); i++)
	{
		AVine *Vine = Cast<AVine>(VineArr[i]);
		actorloc = Vine->GetActorLocation();
		//TRACE("actorlocation       %d", i);
		vec = actorloc - pos;
		Vectorlen = vec.Size();
		//TRACE("Vectorlen %f", Vectorlen);
		if (Vectorlen<a)	
		{
			//移除植物动作
			VineArr[i]->Destroy();
			return;
		}
	}

	//找到最近位置的Cluster植物，移除
	TArray<AActor*> ClusterArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACluster::StaticClass(), ClusterArr);
	for (int i = 0; i < ClusterArr.Num(); i++)
	{
		ACluster *Cluster = Cast<ACluster>(ClusterArr[i]);
		actorloc = Cluster->GetActorLocation();
		//TRACE("actorlocation       %d", i);
		vec = actorloc - pos;
		Vectorlen = vec.Size();
		if (Vectorlen<a)
		{
			//移除植物动作
			ClusterArr[i]->Destroy();
			return;
		}
	}
}

//判断能量是否充足以及执行铁锹的功能移除植物释放位置
bool AMyPlayerController::BEnergyEnoughAndDestroy(int Planter, UUserWidget* EnergyNot)
{
	if (Planter == 0)
	{
		TArray<AActor*> array;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATree::StaticClass(), array);
		ATree *Tree = Cast<ATree>(array[0]);
		if (Tree->TreeEnergyNeed>Energy)
		{
			EnergynotEnoughWidget(EnergyNot);
			PlanterNum = 5;
			return false;			
		}
	}
	else if (Planter == 1)
	{
		TArray<AActor*> array;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVine::StaticClass(), array);
		AVine *Vine = Cast<AVine>(array[0]);
		if (Vine->VineEnergyNeed>Energy)
		{
			EnergynotEnoughWidget(EnergyNot);
			PlanterNum = 5;
			return false;			
		}
	}
	else if (Planter == 2)
	{
		TArray<AActor*> array;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACluster::StaticClass(), array);
		ACluster *Cluster = Cast<ACluster>(array[0]);
		if (Cluster->ClusterEnergyNeed>Energy)
		{
			EnergynotEnoughWidget(EnergyNot);
			PlanterNum = 5;
			return false;			
		}
	}
	else
	{	//移除植物，释放位置
		//TRACE("PAST        %d", TickTime);
		ShovelsFun();
		//TRACE("THEN        %d", TickTime);
		//PlanterNum = 5;
		return false;
	}
	return true;
}

FVector AMyPlayerController::FindMousePosition()
{
	//	FVector Zero(0, 0, 0);	
		////鼠标位置方法1
		//FHitResult TraceHitResult;
		//GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, TraceHitResult);
		////TraceHitResult.TraceEnd
		//if (TraceHitResult.Location !=Zero)
		//{
		//	Energy += 10000;
		//}

		//鼠标位置方法2
	FHitResult bTraceHitResult;
		/*GetHitResultUnderCursor(ECC_Visibility, true, bTraceHitResult1);
		TRACE("00000000%f", bTraceHitResult1.Location.X);
		TRACE("00000000000%f", bTraceHitResult1.Location.Y);
		TRACE("00000000000%f", bTraceHitResult1.Location.Z);
		GetHitResultUnderCursor(ECC_Visibility, true, bTraceHitResult2);
		TRACE("1111111%f", bTraceHitResult2.Location.X);
		TRACE("11111111111%f", bTraceHitResult2.Location.Y);
		TRACE("11111111%f", bTraceHitResult2.Location.Z);*/
	GetHitResultUnderCursor(ECC_Visibility, true, bTraceHitResult);
	TickTime++;
		//TRACE("%d",TickTime);

		//if (bTraceHitResult.Location == FVector(0, 0, 0))
		//{
		//	TRACE("Hit location is zero!");
		//}
		//else
		//{
		//	AActor *Temp = bTraceHitResult.GetActor();
		//	FString Tempchar = Temp->GetName();
		//	//TRACE("Hit Object : %s", *Tempchar);
		//	/*TRACE("%f", bTraceHitResult.Location.X);
		//	TRACE("%f", bTraceHitResult.Location.Y);
		//	TRACE("%f", bTraceHitResult.Location.Z);*/
		//}

		/*for (int i = 0; i < 2; i++)		
		{
			if (bTraceHitResult.Location == FVector(0, 0, 0))
			{
				i++;
				GetHitResultUnderCursor(ECC_Visibility, true, bTraceHitResult);
				TRACE("Hit location is zero!");
			}
			else
			{
				AActor *Temp = bTraceHitResult.GetActor();
				FString Tempchar = Temp->GetName();
				TRACE("%s", *Tempchar);
				TRACE("%f", bTraceHitResult.Location.X);
				TRACE("%f", bTraceHitResult.Location.Y);
				TRACE("%f", bTraceHitResult.Location.Z);
				break;
			}
		}*/
		/*TRACE("222222222%f", bTraceHitResult.Location.X);
		TRACE("2222222222%f", bTraceHitResult.Location.Y);
		TRACE("222222222%f", bTraceHitResult.Location.Z);*/
		/*AActor *Temp2 = bTraceHitResult.GetActor();
		FString Tempchar2 = Temp2->GetName();
		TRACE("%s", *Tempchar2);*/

	//	//if (!bTraceHitResult.bBlockingHit)
	//	//{
	//	//	TRACE("HitResult is Null");
	//	//}
	//	//else
	//	//{
	//	//	//bTraceHitResult.Location;
	//	//	//FVector CursorFV = bTraceHitResult.ImpactNormal;
	//	//	AActor *Temp = bTraceHitResult.GetActor();
	//	//	FString Tempchar = Temp->GetName();
	//	//	TRACE("%s", *Tempchar);
	//	//}

	//	////鼠标位置方法3
	//	//FHitResult HitResult,Start, Dir, End, ;
	//	//DeprojectMousePositionToWorld(Start, Dir);
	//	//End = Start + (Dir*8000.0f);
	//	//GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	//	//if (HitResult.Location!=Zero)
	//	//{
	//	//	ACube* HitCube = Cast<ACube>(HitResult.Actor.Get());
	//	//	Energy += 200000;
	//	//	if (HitCube->Occupied == false)
	//	//	{
	//	//		//种植植物
	//	//		//HitCube->GetActorLocation();
	//	//		//ATree * Tree = GetWorld()->SpawnActor<ATree>(HitCube->GetActorLocation(), FRotator(0, 0, 0));
	//	//		//SetActorLocation(HitCube->GetActorLocation());
	//	//		HitCube->Occupied = true;
	//	//	}
	//	//}

	//	pos = bTraceHitResult.Location;
	//}
	return bTraceHitResult.Location;
}

//能量消耗
void AMyPlayerController::UseEnergy(int Planter,FVector pos)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		BPlant = true;
		if (Planter == 0)
		{
			World->SpawnActor<ATree>(pos,FRotator(0,0,0));
			TArray<AActor*> array;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATree::StaticClass(), array);
			ATree *Tree = Cast<ATree>(array[0]);
			Energy -= Tree->TreeEnergyNeed;
			PlanterNum = 5;
			//TRACE("Plant Tree then use energy");
		}
		else if (Planter == 1)
		{
			World->SpawnActor<AVine>(pos, FRotator(0, 0, 0));
			TArray<AActor*> array;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVine::StaticClass(), array);
			AVine *Vine = Cast<AVine>(array[0]);
			Energy -= Vine->VineEnergyNeed;
			PlanterNum = 5;
		}
		else if (Planter == 2)
		{
			World->SpawnActor<ACluster>(pos, FRotator(0, 0, 0));
			TArray<AActor*> array;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACluster::StaticClass(), array);
			ACluster *Cluster = Cast<ACluster>(array[0]);
			Energy -= Cluster->ClusterEnergyNeed;
			PlanterNum = 5;
		}
	}//World != NULL
	/*LocToBP = FVector(0,0,0);*/
}

void AMyPlayerController::CreateAnimals()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		TArray<AActor*> arr;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACylinder::StaticClass(), arr);

		for (int i = 0; i < 5; i++)
		{
			if (i == FMath::RandHelper(5))
			{
				//TRACE("equel  %d", i);
				ACylinder *Cylinder = Cast<ACylinder>(arr[i]);
				int num = FMath::RandHelper(5);
				FVector pos = Cylinder->GetActorLocation();
				pos.Z += 20;
				switch (num)
				{
				case 1:
					//TRACE("eagle    %d", num);
					World->SpawnActor<AEagle>(pos, FRotator(0, 0, 0));
					break;
				case 2:
					//TRACE("robot    %d", num);
					World->SpawnActor<ARobot>(pos, FRotator(0, 0, 0));
					break;
				case 3:
					//TRACE("Warrok    %d", num);
					World->SpawnActor<AWarrok>(pos, FRotator(0, 0, 0));
					break;
				default:
					break;
				}
			}
		}
	}
}

int AMyPlayerController::Change()
{
	//TRACE(" return     %d",index);
	return index1;
}

int AMyPlayerController::ChangeShovels()
{
	return index2;
}

FVector AMyPlayerController::FindNearestCubeLocation(FVector pos)
{
	//TRACE("Findpos");
	float Length=10000, Vectorlen;
	int i;
	static int rec1;
	FVector actorloc,vec;

	TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACube::StaticClass(), arr);

	for (i = 0; i < 40; i++) //找到最近位置的Cube，记录index1
	{
		ACube *Cube = Cast<ACube>(arr[i]);
		//FString str = Cube->GetName();
		//TRACE("actorlocation       %s", *str);
		actorloc = Cube->GetActorLocation();
		//TRACE("actorlocation       %d", i);
		vec = actorloc - pos;
		Vectorlen = vec.Size();
		if (Vectorlen<Length)
		{
			Length = Vectorlen;
			rec1 = i;
			//TRACE("length:%f", Length);
		}
	}

	index1 = rec1;		//利用蓝图占用
	//TRACE("index    %d",index);
	ACube *Cube = Cast<ACube>(arr[index1]);  
	if (!Cube->Occupied)				//如果没被占领
	{
		//Cube->Occupied;
		//TRACE("don't occupy");
		//index = 99;
		return Cube->GetActorLocation();	//通过返回值种植
	}
	else
	{
		//TRACE("OCCupied");
		//index = 99;
		return FVector(0,0,0);
	}
}

void AMyPlayerController::PictureFollowMouse(bool BClickPlanterRef, UImage* PlanterPictureRef)
{
	BClickPlanter = BClickPlanterRef;
	PlanterPicture = PlanterPictureRef;
	if (BClickPlanter&&PlanterPicture&& !BPauseClick)
	{
		UWidgetLayoutLibrary *myWidget = Cast<UWidgetLayoutLibrary>(UGameplayStatics::GetGameInstance(GetWorld()));
		FVector2D mouseposofview = myWidget->GetMousePositionOnViewport(GetWorld());
		PlanterPictureRef->SetRenderTranslation(mouseposofview);

		//PlanterPictureRef->SetRenderTranslation(FVector2D(i,i));
	}
}

void AMyPlayerController::PictureBack(bool BClickPlanterRef, UImage * PlanterPictureRef)
{
	BClickPlanter = BClickPlanterRef;
	PlanterPicture = PlanterPictureRef;
	if (!BClickPlanter&&PlanterPicture)	//如果松开鼠标并且图片还跟着鼠标
	{
		PlanterPictureRef->SetRenderTranslation(FVector2D(0,0));	//放回原地
	}
}

//种植植物
void AMyPlayerController::PlantAndDestroy(bool BReleasePlanterRef, int Planter,UUserWidget *EnergyNot)
{
	PlanterNum = Planter;
	BReleasePlanter = BReleasePlanterRef;
	if (BReleasePlanterRef&&PlanterNum!=5)	//如果松开鼠标或者植物传参为空，然后判断能量是不是足够或者是不是铁锹
	{
		if (BEnergyEnoughAndDestroy(Planter, EnergyNot))	//如果能量足够，并且筛除铁锹选项，接下来就判断位置是否可用于种植，可以种植则直接种植
		{	//TRACE("EnergyEnough.");
			if (TickTime < 9)  //第三次tick找到鼠标位置才有效，所以设置ticktime计次，我也不知道为啥
			{	//TRACE("TickTime < 8");
				if (FindMousePosition() != FVector(0, 0, 0) && FindMousePosition().X >= -735 && FindMousePosition().X <= 850 && FindMousePosition().Y >= -2150 && FindMousePosition().Y <= 530)	//根据返回值判断是否成功种植  850，-735      -2150  530
				{	//TRACE("mouse pos can find.");
					if (TickTime > 8)
					{
						PlanterNum = 5;
						TickTime = 0;
						//TRACE("overtime");
					}
					else
					{
						FVector pos = FindMousePosition();

						//找最近的位置
						FVector PlantPos = FindNearestCubeLocation(pos);
						if (PlantPos!=FVector(0,0,0))
						{
							UseEnergy(Planter, PlantPos);	//种植并减少相应能量
							PlanterNum = 5;
							//TRACE("USEENERGY because not occu");
							TickTime = 0;
							LocToBP = PlantPos;
							/*TRACE("x      %f", LocToBP.X);
							TRACE("y      %f", LocToBP.Y);
							TRACE("z      %f", LocToBP.Z);*/
						}
						else
						{
							//TRACE("can't planter because occu");
							PlanterNum = 5;
							TickTime = 0;
						}
					}
				}
				//else
				//{
				//	//TRACE("mouse pos error.");
				//}
			}
			else
			{
				//TRACE("TickTime >= 8");
				TickTime = 0; 
				PlanterNum = 5;
			}
		}
	}
}

//图片提醒能量不足
void AMyPlayerController::EnergynotEnoughWidget(UUserWidget * WidgetClass)
{
	if (WidgetClass !=nullptr && BDestroy)
	{
		EnergyNotEnough = WidgetClass;
		BDestroy = false;		//正在销毁
		EnergyNotEnough->AddToViewport();
		GetWorldTimerManager().SetTimer(DelayTime, this, &AMyPlayerController::DestroyEnergyWidget, 2.0f, true);
	}
}