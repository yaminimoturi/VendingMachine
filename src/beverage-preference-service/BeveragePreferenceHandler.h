#ifndef VENDING_MACHINE_MICROSERVICES_BeveragePreferenceHandler_H
#define VENDING_MACHINE_MICROSERVICES_BeveragePreferenceHandler_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/OrderBeverageService.h"
!#include "../../gen-cpp/WeatherService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine{

class BeveragePreferenceServiceHandler : public BeveragePreferenceServiceIf {
	 public:
	   BeveragePreferenceServiceHandler(					   		  ClientPool<ThriftClient<OrderBeverageClient>> *) ;
			     ~BeveragePreferenceServiceHandler() override=default;
	void PlaceOrder(std::string& _return, const int64_t city) override;
        private:
	         ClientPool<ThriftClient<WeatherServiceClient>> *_weather_client_pool;
	};

	// Constructor
	BeveragePreferenceServiceHandler::BeveragePreferenceServiceHandler(
	 		ClientPool<ThriftClient<WeatherServiceClient>> *weather_client_pool) {
	// Storing the clientpool
      _weather_client_pool = weather_client_pool; 		          }
	
          // Remote Procedure "PlaceOrder"
       void BeveragePreferenceServiceHandler::PlaceOrder(std::string& _return, const int64_t city){
	       // Your implementation goes here
                   printf("PlaceOrder\n");

	   //return BeverageType::type::COLD;
  #if 1   
        // 1. get the weather service client pool
           auto weather_client_wrapper = _weather_client_pool->Pop(); 		            if (!weather_client_wrapper) {
	   ServiceException se;
	   se.message="failed";
	   se.message = "Failed to connect to weather-service";
	   throw se;
	   }
		
	   auto weather_client = weather_client_wrapper->GetClient();
	WeatherType::type weatherType = WeatherType::type::COLD;
	 		                                     
	// 2. call the remote procedure : GetWeather

	try {
		weatherType = weather_client->GetWeather(city);
	} catch (...) {

	 	
		_weather_client_pool->Push(weather_client_wrapper);

		LOG(error) << "Failed to send call GetWeather to weather-client";

		throw;

	}

	_weather_client_pool->Push(weather_client_wrapper);



	// 3. business logi


	if(weatherType == WeatherType::type::WARM) 
		_return = "Cold beverage";//BeverageType::type: 		            
	else
	_return = "Hot beverage";//BeverageType::type::HOT;		                
#endif	 		                                         
       }

} // namespace vending_machi
#endif //VENDING_MACHINE_MICROSERVICES_BeveragePreferenceHandler_H
