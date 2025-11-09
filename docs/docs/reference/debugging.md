# Debugging

## Logging

By default, the extension will log to the `nautilus-avinfo` log domain. Debug and info logs are suppressed by default. To enable logs, set the `G_MESSAGES_DEBUG=nautilus-avinfo` or `DEBUG_INVOCATION=1` environment variable when running `nautilus`.

## File Attributes

To view all file attributes associated with a file, run the following command.

```bash
gio info --attributes='*' /path/to/file
```
