#import <Cocoa/Cocoa.h>
#include <stdlib.h>
#include <unistd.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> {
    NSWindow* window;
}
@end

@implementation AppDelegate : NSObject
- (id)init {
    if (self = [super init]) {
        window = [NSWindow.alloc initWithContentRect: NSMakeRect(0, 0, 100, 50)
                                           styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                                             backing: NSBackingStoreBuffered
                                               defer: NO];

        [window setIsVisible: NO];

		[NSTimer scheduledTimerWithTimeInterval: 1.0
										 target: self
									   selector: @selector(timeOut:)
									   userInfo: nil
										repeats: NO];

    }
    return self;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
	//NSLog(@"RadJavVMLauncher: Application starting");
	
    window.title = NSProcessInfo.processInfo.processName;
    [window cascadeTopLeftFromPoint: NSMakePoint(20,20)];
    [window makeKeyAndOrderFront: self];
}

-(void)applicationWillTerminate:(NSNotification *)notification {
	//NSLog(@"RadJavVMLauncher: terminating...");
}

-(void)applicationDidFinishLaunching:(NSNotification *)notification {
	//NSLog(@"RadJavVMLauncher: Application started");
}

- (void)timeOut:(NSTimer*)timer {
	//NSLog(@"RadJavVMLauncher: Timer event");
	//NSLog(@"RadJavVMLauncher: Timer fired no openFile message received");
	
	//[self showAlert:@"No arguments specified. Exiting..."];
	
	//Terminating application
	//NSLog(@"RadJavVMLauncher: Terminating self");
	//[[NSApplication sharedApplication] terminate:nil];

    NSString *executablePath = [[[NSBundle mainBundle] executablePath] stringByDeletingLastPathComponent];
    NSString *examplesPath = [executablePath stringByAppendingString: @"/../Resources/examples/exampleBrowser.xrj"];

    //NSLog(examplesPath);

    [self runApp: examplesPath];
}

- (void) showAlert:(NSString*)message {
	NSAlert *alert = [[NSAlert alloc] init];
	[alert setMessageText:@"RadJavVMLauncher"];
	[alert setInformativeText:message];
	[alert addButtonWithTitle:@"Ok"];
	[alert runModal];
	
	[alert release];
}

- (BOOL) runApp:(NSString*)filename {
    NSString* executablePath = [[[NSBundle mainBundle] executablePath] stringByDeletingLastPathComponent];
    NSString* execCommand = [executablePath stringByAppendingString:@"/RadJavVM"];
    /*execCommand = [@"file:///" stringByAppendingString: executablePath];
    NSArray<NSString *> *args = @[filename];

    NSURL *url = [[NSURL alloc] initFileURLWithPath: execCommand];
    [NSTask launchedTaskWithExecutableURL: url arguments: args error: nil terminationHandler:nil];
    
    [url release];*/

    const char* execCommandUtf8 = [execCommand cStringUsingEncoding:NSUTF8StringEncoding];
    const char* parameterUtf8 = [filename cStringUsingEncoding:NSUTF8StringEncoding];
    pid_t processID = fork();
    
    if(processID < 0)
    {
        //Unable to create a new process
        //NSLog(@"RadJavVMLauncher: Unable to start application");
        [self showAlert:@"Unable to start application. Exiting..."];
        
        //Terminating application
        [[NSApplication sharedApplication] terminate:nil];
        
        return true;
    }
    
    if(processID > 0)
    {
        //Child created successfully
        //NSLog(@"RadJavVMLauncher: Child process created successfully");
        
        //Terminating application
        [[NSApplication sharedApplication] terminate:nil];
        
        return true;
    }
    
    if (processID == 0)
    {
        //We are in the child process
        //Replacing image with actual program
        if( execl( execCommandUtf8, execCommandUtf8, parameterUtf8, NULL) == -1)
            _exit(EXIT_FAILURE);
    }

    return (true);
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename {
	//NSLog(@"RadJavVMLauncher: openFile event received with argument: %@", filename);

	//As an option
	/*
	NSTask *task = [[NSTask alloc] init];
	task.launchPath = execCommand;
	task.arguments = @[filename];
	
	[task launch];
	[task release];
	 */

    BOOL result = [self runApp: filename];

	return result;
}

@end

int main(int argc, const char * argv[]) {
	//Getting application
	NSApplication* app = NSApplication.sharedApplication;
	
	//Set activation policy
	app.ActivationPolicy = NSApplicationActivationPolicyRegular;
	
	//Creating main menu
	NSMenuItem* item = NSMenuItem.new;
	NSApp.mainMenu = NSMenu.new;
	item.submenu = NSMenu.new;
	[app.mainMenu addItem: item];
	[item.submenu addItem: [[NSMenuItem alloc] initWithTitle: [@"Quit " stringByAppendingString: NSProcessInfo.processInfo.processName] action:@selector(terminate:) keyEquivalent:@"q"]];
	
	//Creating application delegate
	AppDelegate* appDelegate = AppDelegate.new;
	
	app.delegate = appDelegate;
	
	//Running main loop
	(void)app.run;
	
    return 0;
}
