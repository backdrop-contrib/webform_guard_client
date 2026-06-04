# Webform Guard Client
**Webform Guard Client** — Protect your Backdrop CMS webforms from spam by routing Webform submissions through a centralised guard service before delivery.

Webform Guard Client is a Backdrop CMS module that integrates with the Backdrop Webform module to intercept submissions before the normal email and notification workflow runs. Each submission is sent to a remote `webform_guard_server` installation for a spam check. If the server identifies the submitter as a known spammer, the submission is silently blocked and the user sees a friendly message. Clean submissions pass through untouched. Recipients can also report unwanted messages as spam directly from a link in their notification email — no account or login required or from the view submissions form.

## Alpha Release Notes
As an alpha release, the core submission checking, spam token generation, email link injection, and admin reporting button are fully functional. Testers should ensure their Backdrop CMS environment is running PHP 8.0+, that the companion `webform_guard_server` module is installed and reachable, and that system caches are cleared after installation to register the new hooks and menu items.  The Webform Guard Server can be run from the same site.

## Features

* **Transparent Submission Checking:** Intercepts webform submissions at the validation stage and checks them against the guard server before any email or notification is sent. If the service is unreachable and fallback mode is enabled, submissions are allowed through automatically.
* **Per-Webform Configuration:** Enable or disable guard checking individually for each webform node. Select which field (typically an email field) is used to identify repeat spam submissions.
* **Configurable Block Message:** Choose whether to show a message to users when their submission is blocked, and customise the message text per webform.
* **Email Spam Report Link:** Automatically appends a *Report as spam* link to outgoing webform notification emails. The link is unique to each submission and expires after 30 days.
* **Admin Report Button:** Adds a *Report as spam* action button to the submission preview page in the Backdrop admin. Clicking it opens an AJAX dialog and relays the report to the guard server without a page reload.
* **Single-Use Tokens:** Spam report links are backed by cryptographically random tokens stored locally. Each token can only be used once and expires automatically.
* **Connection Test:** The settings page includes a *Test connection* button that calls the guard server's status endpoint and displays the authentication result and server version.
* **Webform Overview:** The module admin page shows a table of all webforms on the site with their guard status and configured identifier field at a glance.
* **Backdrop Native:** Built exclusively for Backdrop CMS using strict PHP 8.0+ standards and Backdrop APIs throughout.

## Requirements

- Backdrop CMS 1.x
- PHP 8.0+
- [Webform module](https://backdropcms.org/project/webform) for Backdrop CMS
- A running installation of the companion [Webform Guard Server](../webform_guard_server) module

## Installation

Install this module using the official Backdrop CMS instructions at https://docs.backdropcms.org/documentation/extend-with-modules

Enable the module on each site you wish to protect.

## Configuration

1. Navigate to **Admin → Configuration → Webform Guard Client → Settings**.
2. Enter the **Guard server base URL** — the base URL of your `webform_guard_server` installation, without a trailing slash (e.g. `https://guard.example.com`). Do not include the API path; the module appends this automatically.
3. Enter the **API key** provided by your guard server administrator.
4. Enter the **Site identifier** — this must match the site identifier configured for this site in the guard server's client registry (recommended: your site's domain, e.g. `mysite.co.uk`).
5. Optionally enable **Allow fallback on service error** to permit submissions through if the guard server is unavailable.
6. Click **Test connection** to verify the settings before saving.
7. To enable guard checking on a specific webform, edit that webform node and locate the **Webform Guard settings** fieldset. Enable the toggle and select the email field used to identify the submitter.

## Issues

Bugs and feature requests should be reported in the Issue Queue: https://github.com/albanycomputers/webform_guard/issues

## Current Maintainer(s)
- Steve Moorhouse (albanycomputers) (https://github.com/albanycomputers)
- Additional maintainers and contributors are welcome.

## Credits
- Steve Moorhouse — Zulip (DrAlbany)
- Claude Code by Anthropic assisted with development of this module.

## Sponsorship
- Albany Computer Services (https://www.albany-computers.co.uk)
- Albany Web Design (https://www.albanywebdesign.co.uk)
- Albany Hosting (https://www.albany-hosting.co.uk)

## License
This project is GPL v2 or later software. See the LICENSE.txt file in this directory for complete text.
