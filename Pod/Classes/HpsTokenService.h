//
//  TokenService.h
//  Heartland-Tokenization
//
//  Created by Roberts, Jerry on 2/2/15.
//  Copyright (c) 2015 Heartland Payment Systems. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HpsTokenData.h"

@interface HpsTokenService : NSObject

- (id) initWithPublicKey:(NSString*)publicKey;

- (void) getTokenWithCardNumber:(NSString*)cardNumber
                            cvc:(NSString*)cvc
                       expMonth:(NSString*)expMonth
                        expYear:(NSString*)expYear
               andResponseBlock:(void(^)(HpsTokenData*))responseBlock;

- (void) getTokenWithCardTrackData:(NSString *)trackData
				  andResponseBlock:(void(^)(HpsTokenData *))responseBlock;

- (void) getTokenWithEncryptedCardTrackData:(NSString *)trackData
								trackNumber:(NSString *)trackNumber
										ktb:(NSString *)ktb
								   pinBlock:(NSString *)pinBlock
						   andResponseBlock:(void(^)(HpsTokenData *))responseBlock;

- (void) getTokenWithCardDictionary:(NSDictionary *)cardDictionary
          orEncryptedCardDictionary:(NSDictionary *)encryptedCardDictionary
                   andResponseBlock:(void(^)(HpsTokenData*))responseBlock;

@end
