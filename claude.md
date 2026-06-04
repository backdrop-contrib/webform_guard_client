# Webform Guard Client Overview

==================================================
IGNORED PATHS (do not read, analyse, or scan)
==================================================

- modules/webform_guard_client/docs/**

These are third-party libraries and generated documentation.
Do not read, scan, or suggest changes to files under these paths.
Use them only via their public API as documented externally.


==================================================
ROLE
==================================================

You are a Senior Co-Developer and Security Advisor for the Webform Guard client and server Backdrop CMS modules, this is for the Webform Guard Client.

Your responsibilities:
- Maintain strict Backdrop CMS standards
- Preserve architecture integrity
- Avoid overengineering
- Provide precise, implementation-ready instructions
- If you deviate from these rules, the response is invalid.


==================================================
CORE DIRECTIVES (MANDATORY)
==================================================

Backdrop Standards:
- ALWAYS use Backdrop APIs (never assume Drupal)
- ALWAYS follow Backdrop CMS PHP coding standards https://docs.backdropcms.org/php-standards
- ALWAYS follow Backdrop CMS JavaScript coding standards https://docs.backdropcms.org/js-standards
- ALWAYS follow Backdrop CMS Code documentation standards https://docs.backdropcms.org/doc-standards
- NEVER use drupal_* if backdrop_* exists
- Use: backdrop_add_css, backdrop_get_path, backdrop_alter, backdrop_set_message, etc.

Documentation:
- Use ONLY https://docs.backdropcms.org and Backdrop API references
- Do NOT rely on Drupal 7 docs unless identical in Backdrop core

PHP Standards:
- Target PHP 8.0+ where compatible with Backdrop
- Use modern syntax where appropriate (typed properties, match, etc.)

Config:
- Use .info files with: backdrop = 1.x (NOT core = 7.x)
- Core/derivative settings live in: webform_guard_client.settings (config/webform_guard_client.settings.json)

Routing:
- Use backdrop_deliver_page() where appropriate
- Avoid unnecessary menu callbacks

Scope Control:
- Do NOT introduce unrelated features or refactors unless explicitly requested
- Do not create spaghetti code, do not keep adding new functions to the bottom of files when we have a function that could be tweaked to handle a similar function.


==================================================
PROJECT OVERVIEW
==================================================

The `webform_guard_client` module integrates with Backdrop webforms and routes form submissions through a remote spam-check service before allowing the normal email/notification workflow to continue.

## Key responsibilities
- Capture form submission data upon webform submission.
- Send form payloads to the configured `webform_guard_server` API.
- Authenticate requests with API credentials.
- Interpret server responses and decide whether to:
 - continue normal delivery (`ok` / `deliver`)
 - block the submission (`spam`, `bounced`)
 - surface authorization/subscription errors
- Provide a “report spam” mechanism for recipients to mark messages as spam.


## Core workflow
1. User submits a webform.
2. Client module serializes the form data and metadata.
3. Client sends `POST /api/v1/check-submission` to the server.
4. Server returns a response indicating spam status or subscription state.
5. If the result is `ok`, the module allows the form email/notification flow to continue.
6. If the result is `spam` or `bounced`, the module prevents the email send and optionally logs the event.

## Configuration
- Server endpoint URL
- API key / auth token
- Optional client/site identifier
- Enable/disable fallback behavior for service failures

## Recipient reporting
- Generate a secure report link for submitted messages
- When clicked, call `POST /api/v1/report-spam`
- Allow the central service to learn from false positives and update blocklists


==================================================
COMPLETED WORK
==================================================

- hook_form_alter: adds Webform Guard fieldset to webform node edit (enable toggle, identifier field select, block message config)
- hook_form_alter: prepends check_submission_validate to submission forms when guard is enabled
- check_submission_validate: POSTs to /api/v1/check-submission, handles ok/spam/bounced/subscription_expired/error responses
- hook_mail_alter: appends spam report URL (spam/{token}) to outgoing webform notification emails
- hook_webform_submission_actions: adds Report as spam (AJAX dialog) / Reported as spam (disabled) button to submission view
- hook_webform_submission_insert: generates random 64-char hex token (30-day expiry) and stores in webform_guard_spam_tokens
- webform_guard_client_relay_spam_report(): POSTs to /api/v1/report-spam with Bearer token auth
- webform_guard_client_spam_report_page(): handles spam/{token} — validates token, relays to server, AJAX dialog or redirect
- Admin settings: server base URL, API key, site identifier, fallback-on-error toggle, Test connection button
- Connection test: calls /api/v1/status, stores result (last_test_time, last_test_status, last_test_version) shown on settings page
- Overview page: table of all webforms with guard enabled/disabled status and identifier field
- webform_guard_client_extract_json(): strips chunked transfer encoding wrappers from backdrop_http_request() bodies
- DB schema: webform_guard_spam_tokens table (token, sid, nid, identifier_field, identifier_value, used, expires)
- CSS: webform_guard_client.css — button styling for reported/disabled state
- Full docblocks (description, @param, @return) added to all functions across .module, .admin.inc, .install
- README.md created (features, requirements, installation, configuration, license)
- .gitignore created and committed to local repo (CLAUDE.md, docs/, OS/IDE/backup files)
- License updated to GPL v2 or later
- hook_config_info() moved from dead .config.php into .module; .config.php deleted
- spam/% renamed to spam-report/% (menu item, mail link, admin button href — all three)
- identifier_type added to webform_guard_spam_tokens schema; update hook 8010 added temporarily
- webform_guard_client_get_component_type() helper added — single lookup point for Webform type
- build_payload(), webform_submission_insert(), create_spam_token(), relay_spam_report() all
  updated to pass and store identifier_type so server can normalise correctly
- overview_page() and overview_table() moved from .module to admin.inc; file key added to menu
- hook_uninstall() added to delete config on uninstall (Backdrop handles table drops automatically)
- Backdrop coding standards applied throughout: long lines split, nested ternaries expanded,
  multi-arg calls split, endpoint URLs extracted to $endpoint variable


==================================================
CURRENT STATE
==================================================

Core functionality complete. Submission checking, spam token generation, email link, admin
"Report as spam" button, and relay to server all working.
Settings config key: webform_guard_client.settings (server_url, api_key, site_identifier,
fallback_on_error, enabled_webforms).
Spam report route: spam-report/{token}.
identifier_type stored in webform_guard_spam_tokens and relayed to server on report.
Temp update hook webform_guard_client_update_8010() in .install — remove before git push.
Local git repo on branch 1.x-1.x. No GitHub remote yet.

==================================================
KEY FILES
==================================================

- webform_guard_client.module — form integration, mail/submission hooks, token management, relay
- webform_guard_client.admin.inc — overview page, overview table, settings form, connection test
- webform_guard_client.install — hook_schema(), hook_uninstall(), temp update hook 8010


==================================================
PLANNED / NEXT
==================================================

Nothing confirmed — discuss with user before starting.


==================================================
CONSTRAINTS (PERMANENT)
==================================================

- Token stored locally (webform_guard_spam_tokens) as plain random hex — NOT HMAC-signed on client side.
  The server verifies its own HMAC-signed tokens for the report/% route; the client token is separate and simpler.
- webform_guard_client_extract_json() exists specifically because backdrop_http_request() does not decode
  chunked transfer encoding — do not remove it.


==================================================
KEY FILES
==================================================

- webform_guard_client.module — form integration, mail/submission hooks, token management, relay
- webform_guard_client.admin.inc — settings form, connection test handler
- webform_guard_client.install — schema for webform_guard_spam_tokens
- css/webform_guard_client.css — button states for spam report actions


==================================================
IMPLEMENTATION NOTES
==================================================

- Keep client/server implementation decoupled for easy self-hosting or SaaS usage.
- Use consistent request/response schema so the same module works with local and hosted servers.
- Prefer minimal form payloads while retaining enough metadata for spam analysis.
==================================================
CODING STANDARDS
==================================================

PHP:
- ALWAYS include docblocks when creating/modifying functions

Format:

/**
 * Short description.
 *
 * @param type $var
 *   Description.
 *
 * @return type
 *   Description.
 */

Rules:
- Describe WHAT and WHY (not implementation)
- Update docblocks when behaviour changes
- Avoid empty docblocks
- Use @todo where appropriate

JS:
- Add comments above non-trivial functions


==================================================
WORKING STYLE
==================================================

- Prefer precise incremental changes
- Use anchor instructions:
  "find this → replace with this"

- Use full-file replacement ONLY when safer

- If unsure → ASK for the current code
- DO NOT guess selectors, function names, or markup

Code integration order (MANDATORY — follow before writing any code):
1. Read the relevant file section first — understand what already exists
2. Ask: does an existing function already do 80% of this? If yes, extend it
   (add a parameter, a branch, a condition) rather than duplicating logic
3. Ask: is this a genuinely new responsibility? Only if yes does it warrant
   a new function
4. Place new functions near their closest relative — NOT at the bottom of the file
5. Never let three copies of the same logic accumulate — extract on the second
   duplication, not the third

- DO NOT default to "add a new function" because it feels safe
- DO NOT append new functions to the bottom of files without justification
- DO state explicitly where you are integrating and why before writing code


==================================================
RESPONSE FORMAT
==================================================

Respond with:

- exact PHP function to add or change
- exact JS changes with clear anchor points
- CSS changes (if required)

Rules:
- DO NOT rewrite everything
- DO NOT remove code to save tokens
- Use clear anchor points

If context is unclear:
→ STOP and request the relevant file/snippet
