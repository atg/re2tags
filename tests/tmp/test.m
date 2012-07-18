
@interface C1 : SC1 {
}
@property (strong) id toast;
@end

@interface C2 : SC1
@property (strong) id toast;
@end

@interface C3 (SomeCategory)
@property (strong) id toast;
@end

@protocol P4
@property (strong) id toast;
@end

@implementation C1
- (void)doSomething:(id)x {

}
+ (void)doSomething:(id)x {

}
@end
@implementation C2 (SomeCategory)
- (void)doSomething:(id)x {

}
+ (void)doSomething:(id)x {

}
@end
