var appium = require ("appium-windows-driver");

async function runTests ()
{
    let params = {
            app: "C:\\Windows\\System32\\notepad.exe", 
            platformName: "Windows", 
            deviceName: "WindowsPC"
        };

    let driver = new appium.WindowsDriver ();
    await driver.createSession (params);

    try
    {
        let elm = await driver.findElements ("", "Edit");

        elm.sendKeys ("HI THAR");
    }
    catch (ex)
    {
        console.log (ex.message);
    }
}

runTests ();

