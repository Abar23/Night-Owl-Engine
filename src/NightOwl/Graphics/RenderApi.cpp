#include "RenderApi.h"

namespace NightOwl::Graphics
{
	RenderApi* RenderApi::renderApi = nullptr;


	RenderApi* NightOwl::Graphics::RenderApi::GetInstance()
	{
		if(renderApi == nullptr)
		{
			renderApi = new RenderApi();
		}
		return renderApi;
	}
}