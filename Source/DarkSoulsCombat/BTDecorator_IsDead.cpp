//// Fill out your copyright notice in the Description page of Project Settings.
//
//#include "BTDecorator_IsDead.h"
//#include "DSCharacter.h"
//#include "DSAIController.h"
//#include "DSCharacterStatComponent.h"
//#include "BehaviorTree/BlackboardComponent.h"
//
//UBTDecorator_IsDead::UBTDecorator_IsDead()
//{
//
//}
//
//
//bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
//{
//	//bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
//
//	//auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
//	//if (nullptr == DSCharacter)
//	//{
//	//	return;
//	//}
//
//	//auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
//	//if (nullptr == Target)
//	//{
//	//	return;
//	//}
//
//	//
//
//	// ���� �̰� �ƴѰͰ����� �ϴ� �̷���... ������ �ǵ���
//	//// ��������Ʈ�� �޾Ƽ� ����� �����̺�Ʈ�� ���� ���ϵ��� ����
//	//pOwnerComp = &OwnerComp;
//
//	//DSCharacter->CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
//	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADSAIController::IsDeadKey, true);
//	//});
//
//	//if (!(pOwnerComp->GetBlackboardComponent()->GetValueAsBool(ADSAIController::IsDeadKey)))
//	//{
//	//	return;
//	//}
//
//	//OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::RunTargetPosKey, (Target->GetActorLocation() - DSCharacter->GetActorForwardVector()));
//
//
//
//	return bResult;
//}