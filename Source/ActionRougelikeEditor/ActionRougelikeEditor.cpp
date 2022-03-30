#include "ActionRougelikeEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FActionRougelikeEditor, ActionRougelikeEditor);

DEFINE_LOG_CATEGORY(ActionRougelikeEditorLog)

#define LOCTEXT_NAMESPACE "ActionRougelikeEditor"

void FActionRougelikeEditor::StartupModule()
{
	UE_LOG(ActionRougelikeEditorLog, Warning, TEXT("ActionRougelikeEditor: Log Started"));
}

void FActionRougelikeEditor::ShutdownModule()
{
	UE_LOG(ActionRougelikeEditorLog, Warning, TEXT("ActionRougelikeEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE