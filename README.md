# LunchBell
An Arduino powered notification system, brings digital notifications to the real world

You'll need to use a gmail account, probably best to create a new account for this since you are putting the login information in the Arduino file

You'll need to create a label and set up rules to forward your emails based on subject line or sender to this label so the Arduino can properly pick it up
To find more information about how to setup gmail properly check out this repo in which this project is HEAVILY based upon: https://github.com/amicojeko/Arduino-Yun-Gmail-Check

This project is using Slack Outbound Webhooks to trigger the notification from Slack: https://api.slack.com/outgoing-webhooks

The PHP file included is where you'll point your URL field in the setup for Slack to send a POST request too. The PHP file simply sends an email to the gmail account (that you specify in the Arduino file)
