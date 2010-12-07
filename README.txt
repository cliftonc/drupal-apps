Overview of the Drupal Apps SDK

The APPS SDK is a very simple implementation of a facebook style apps framework, with the only difference being that it is up to the site administrator to configure the applications rather than the users (though this user driven model is eminently possible if anyone wants to take up the mantle!).

It works as follows:

Install the Apps SDK Module in your drupal site, download from here.

Via the administration area, add a new app, with:

Name: This appears in the navigation menu.
Key: This is a shared key that must be used when configuring the app javascript SDK in later steps - note it down.
URL: The base url of your application (this is the first page the iFrame links to).
CSS: A CSS file that can be applied to the application, must be an absolute URL as it is passed through to the child app to over-ride its style.
Status: Use to enable / disable apps.

Download the Apps SDK from here: App Javascript SDK

Add the javascript folder to your application, and include it as per the example (child.html) in the zip.
Configure the initialisation parameters of the application (as per the example) to match your parent domain (e.g. the base url of your drupal site), and the key that was created earlier when you created the app.

The app should now work when clicked on.

You can now edit the composite.parent and composite.child javascript on both sides of the divide to implement shared functions, e.g. allow the child to call javascript functions on the parent (e.g. to see user information) or vice versa. In the example, clicking on the user name in the child application triggers a modal dialog that is defined in the parent to appear, passing information back to enable drupal to provide information on the user ... think employee lookup, presence detection, chat, etc. across all enterprise apps that you can stick the SDK into and have a key (email or username) that is common.

More detailed examples to follow ...