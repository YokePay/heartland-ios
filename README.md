# Heartland-iOS-SDK

[![CI Status](http://img.shields.io/travis/Shaunti Fondrisi/Heartland-iOS-SDK.svg?style=flat)](https://travis-ci.org/Shaunti Fondrisi/Heartland-iOS-SDK)
[![Version](https://img.shields.io/cocoapods/v/Heartland-iOS-SDK.svg?style=flat)](http://cocoapods.org/pods/Heartland-iOS-SDK)
[![License](https://img.shields.io/cocoapods/l/Heartland-iOS-SDK.svg?style=flat)](http://cocoapods.org/pods/Heartland-iOS-SDK)
[![Platform](https://img.shields.io/cocoapods/p/Heartland-iOS-SDK.svg?style=flat)](http://cocoapods.org/pods/Heartland-iOS-SDK)

## Usage

To run the example project, clone the repo, and run `pod install` from the Example directory first.

## Requirements
Adding this pod will also include the XMLDictionary, and Masonry cocoapod as a dependency.
 
## Installation

Heartland-iOS-SDK is available through [CocoaPods](http://cocoapods.org). To install
it, simply add the following line to your Podfile and run "pod update":

```ruby
pod 'Heartland-iOS-SDK'
```

#####For iOS 9 only: Whitelist heartlandportico.com

If you compile your app with iOS SDK 9.0 or above, you will be affected by App Transport Security. Currently, you will need to whitelist heartlandportico.com in your app by adding the following to your application's plist:

```Objective-C
<key>NSAppTransportSecurity</key>  
<dict>  
<key>NSExceptionDomains</key>  
  	<dict>  
    	<key>heartlandportico.com</key>  
	    <dict>  
		    <key>NSIncludesSubdomains</key>  
		    <true/>  
		    <key>NSTemporaryExceptionAllowsInsecureHTTPLoads</key>  
		    <true/>  
		    <key>NSTemporaryExceptionMinimumTLSVersion</key>  
		    <string>1.0</string>  
		    <key>NSTemporaryExceptionRequiresForwardSecrecy</key>  
		    <false/>  
	  	</dict>  
	</dict>  
</dict>  

```
## Heartland SecureSubmit Tokenization for iOS
>This project makes it easy to convert sensitive card data to single-use payment tokens 


#### Simple TokenService
Below is an example of all that is required to convert sensitive card information into a single-use token.  The request is asynchronous so you can safely run this code on the UI thread.
```objective-c
    HpsTokenService *service = [[HpsTokenService alloc] initWithPublicKey:@"YOUR PUBLIC KEY GOES HERE"];
    
    [service getTokenWithCardNumber:@"4242424242424242"
                                cvc:@"012"
                           expMonth:@"3"
                            expYear:@"2017"
                   andResponseBlock:^(HpsTokenData *tokenResponse) {
                       
                       if([tokenResponse.type isEqualToString:@"error"]) {
                            self.tokenCodeResultLabel.text = tokenResponse.code;
                            self.tokenResultLabel.text = tokenResponse.message;
                       }
                       else {
                            self.tokenResultLabel.text = tokenResponse.tokenValue;
                       }
                       
                   }];
```

#### Simple Gateway Service
Below is an example of all that is required to run a card transaction directly.


```objective-c
     // 1.) Configure the service
    HpsServicesConfig *config = [[HpsServicesConfig alloc]
                                 initWithSecretApiKey: mySuperSecretKey
                                 developerId: @"123456"
                                 versionNumber: @"1234"];
    
    // 2.) Initialize the service
    HpsGatewayService *service = [[HpsGatewayService alloc] initWithConfig:config];
    
    // 3.) Initialize a Transaction to process
    HpsTransaction *transaction = [[HpsTransaction alloc] init];
    
    //card holder data
    transaction.cardHolderData.firstName = @"Jane";
    transaction.cardHolderData.lastName = @"Doe";
    transaction.cardHolderData.address = @"123 Someway St.";
    transaction.cardHolderData.city = @"Anytown";
    transaction.cardHolderData.zip = @"AZ";


    //***************************
    //add the tokenResponse to charge against.
    transaction.cardData.tokenResponse = tokenResponse;
    //***************************

    //***************************
    //Or charge by using card data directly - Not the recommended way for processing. 
    //transaction.cardData.cardNumber = @"4242424242424242";
    //transaction.cardData.expYear = @"2021";
    //transaction.cardData.expMonth = @"6";
    //***************************

    
    //Do you want a re-use token returned?
    transaction.cardData.requestToken = YES;
    
    //charge details
    transaction.chargeAmount = 24.50f;
    
    //additional details (optional)
    transaction.additionalTxnFields.desc = @"Mega Sale";
    transaction.additionalTxnFields.invoiceNumber = @"12345";
    transaction.additionalTxnFields.customerID = @"4321";
    
    // 4.) Run the transaction with the service.
    [service doTransaction:transaction
         withResponseBlock:^(HpsGatewayData *gatewayResponse, NSError *error) {
             
             if (error != nil) {
                 
      
                //Houston we have a problem
                 
                //You can look at the 
                //Processing error codes:

                // [error localizedDescription];
                 

                //gatewayResponse.responseCode
                //gatewayResponse.responseText

                //gatewayResponse.gatewayResponseCode
                //gatewayResponse.gatewayResponseText
                 
                
             }else{
                //success


             }
         }];

```

#### Orca Device Service
Below is an example of how the call the Orca services for API key retrieval and device activation.

```objective-c
     
    //Device activation request - Emails a code to the address entered below.
    // 1.) Configure the service data
    HpsServicesConfig *config = [[HpsServicesConfig alloc]init];
    
    config.userName = @"admin";
    config.password = @"password";
    config.isForTesting = YES;  //remove for production
    
    HpsDeviceData *device = [[HpsDeviceData alloc] init];
    device.merchantId = @"xxxxxxxx";
    device.deviceId = @"xxxxxxxxx";
    device.email = @"username@website.com";
    device.applicationId = @"Mobuyle Retail";
    device.hardwareTypeName = @"Heartland Mobuyle";
    
    //optional
    device.softwareVersion = @"";
    device.configurationName = @"";
    device.peripheralName = @"";
    device.peripheralSoftware = @"";
    
    // 2.) Initialize the service
    HpsOrcaService *service = [[HpsOrcaService alloc] init];
    
    // 3.) Call
    [service deviceActivationRequest:device withConfig:config andResponseBlock:^(HpsDeviceData *deviceData, NSError *error) {
 
        if (error != nil) {
            //failed
            
            NSString *errorMessage = [error localizedDescription];
            
        }else{
            //success
            //deviceData has the results, but the activation code is in the email.
            
        }
        
    }];
    
    
    //Device activation request - From the activation code in your email.
    // 1.) Configure the service data
    HpsServicesConfig *config = [[HpsServicesConfig alloc]init];
    config.isForTesting = YES; //remove for production
    
    HpsDeviceData *device = [[HpsDeviceData alloc] init];
    device.merchantId = @"xxxxx";
    device.applicationId = @"Mobuyle Retail";
    device.activationCode = @"xxxxxxx";
    
    // 2.) Initialize the service
    HpsOrcaService *service = [[HpsOrcaService alloc] init];
    
    // 3.) Call
    [service activeDevice:device withConfig:config andResponseBlock:^(HpsDeviceData *deviceData, NSError *error) {
        if (error != nil) {
            //failed
            
            NSString *errorMessage = [error localizedDescription];
            
        }else{
            //success
            //deviceData has the results.
            
            //Here is your secret key.  Store and retrieve this securely.
            // deviceData.apiKey;
        }
        
    }];
    
    
    
    //Fetch your secret api key.
    // 1.) Configure the service data
    HpsServicesConfig *config = [[HpsServicesConfig alloc]init];
    config.siteId = @"xxxxx";
    config.licenseId = @"xxxxxx";
    config.userName = @"admin";
    config.password = @"password";
    
    config.isForTesting = YES;  //remove for production

    config.deviceId = @"xxxxxx";
    
    
    // 2.) Initialize the service
    HpsOrcaService *service = [[HpsOrcaService alloc] init];
    
    // 3.) Call
    [service getDeviceAPIKey:config andResponseBlock:^(NSString *apiKey, NSError *error) {
        
        if (error != nil) {
            //failed
            
            NSString *errorMessage = [error localizedDescription];
            
        }else{
            //success
            //Here is your secret key.  Store and retrieve this securely.
            // apiKey;
        }
        
    }];
    
    
    //Device activation request - Eamils a code to the address entered below.
    // 1.) Configure the service data
    HpsServicesConfig *config = [[HpsServicesConfig alloc] init];
    config.secretApiKey = @"YOUR SECRET API KEY"
    config.isForTesting = YES;   //remove for production
    
    HpsDeviceData *device = [[HpsDeviceData alloc] init];
    device.deviceId = @"xxxxxxxxx";
    device.applicationId = @"Mobuyle Retail";
    device.hardwareTypeName = @"Heartland Mobuyle";
    
    // 2.) Initialize the service
    HpsOrcaService *service = [[HpsOrcaService alloc] init];
    
    // 3.) Call
    [service getDeviceParameters:device withConfig:config andResponseBlock:^(NSDictionary *payload, NSError *error) {
        if (error != nil) {
            //failed
            
            NSString *errorMessage = [error localizedDescription];
            
        }else{
            //success
            //payload has the results.
            
        }
        
    }];

```

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
[DeveloperPortal]: https://developer.heartlandpaymentsystems.com/securesubmit


## License

Heartland-iOS-SDK is available under the EULA license. See the LICENSE file for more info.
