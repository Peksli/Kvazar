#include "Application.h"
#include "LogSystem.h"


class UserApp : public Kvazar::Application
{

};

Kvazar::Application* Kvazar::CreateApplication()
{
	return new UserApp();
}


int main()
{
	Kvazar::LogSystem::Initialize();
	CLIENT_INFO("Hello from client");

	auto userApp = Kvazar::CreateApplication();

	userApp->Init();
	userApp->Run();
	userApp->Shutdown();

	delete userApp;

	return 0;
}