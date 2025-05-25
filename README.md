# Command Group Documentation

Command Group is a lightweight alternative to aliases, allowing you to combine multiple commands into a single, reusable command for streamlined workflows. Below is a detailed guide on how to use Command Group effectively.

## Overview

With Command Group, you can group frequently used commands and execute them as a single command, simplifying repetitive tasks.

## Managing Groups

### Adding a Group
Create a new command group with the following syntax:

```bash
cg add group <group-name>
```

**Alias:**
```bash
cg -ag <group-name>
```

### Removing a Group
Delete an existing command group:

```bash
cg remove group <group-name>
```

**Alias:**
```bash
cg -rg <group-name>
```

## Managing Commands

### Adding a Command
Add a command to an existing group:

```bash
cg add command <group-name> <command-name> "<command-to-execute>"
```

**Alias:**
```bash
cg -ac <group-name> <command-name> "<command-to-execute>"
```

### Removing a Command
Remove a command from a group:

```bash
cg remove command <group-name> <command-name> "<command-to-execute>"
```

**Alias:**
```bash
cg -rc <group-name> <command-name> "<command-to-execute>"
```

### Executing a Command
Run a command from a group:

```bash
cg <group-name> <command-name>
```

## Example Usage

Here’s a complete example of using Command Group:

```bash
# Create a group named "brew"
cg add group brew

# Add a command to the "brew" group
cg add command brew update "brew update && brew upgrade"

# Execute the command
cg brew update
```

## Tips for Use
- Ensure command names are unique within a group for clarity.
- Use descriptive group and command names to easily remember their purpose.