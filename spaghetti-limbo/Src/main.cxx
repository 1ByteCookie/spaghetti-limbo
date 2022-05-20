#include "Application.hpp"

int main()
{
	Application* This = new Application;
	int Status = This->OnStart();
	delete This;

	return Status;
}