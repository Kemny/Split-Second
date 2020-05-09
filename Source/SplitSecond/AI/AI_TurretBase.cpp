// This project falls under CC-BY-SA lisence


#include "AI_TurretBase.h"
#include "../AI/AI_TraceComponent.h"

AAI_TurretBase::AAI_TurretBase()
{
  AITraceComp = CreateDefaultSubobject<UAI_TraceComponent>(TEXT("AITraceComp"));
}
