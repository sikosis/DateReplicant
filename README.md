<p align="center">
  <img src="resources/icon.png" alt="icon" width="256">
</p>

<h1 align="center">DateReplicant</h1>

<p align="center">
  <img src="https://img.shields.io/badge/version-v0.1-f5a623" alt="Version v0.1">
  <img src="https://img.shields.io/badge/platform-Haiku-ffcc00" alt="Platform: Haiku">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599c" alt="C++17">
</p>

DateReplicant is a small Haiku application with two Deskbar displays: the
original graphical replicant shows the day, month and year over a configurable
background, while a compact text item shows the complete date directly in
Deskbar. It modernises Vlad B.'s original Date 0.01 while retaining its settings
format and application signature. The untouched historical source is kept in
[`DateReplicant-Original`](DateReplicant-Original).

## Build

DateReplicant uses Haiku's standard `makefile-engine` and requires Haiku with
the development tools installed:

```sh
make
```

The resulting `DateReplicant` application can be launched normally. It installs
the text date in Deskbar on first launch without adding duplicates. Use the
graphical view's dragger to add the original display to Deskbar as well. The
compact item uses the locale-aware format `%a %d %b %Y`, for example
`Fri 23 Oct 2026`.

## Configuration

On first launch, DateReplicant creates
`~/config/settings/Date_settings`. The file controls the AM and PM background
images, fonts, colours, positions, visibility and `strftime` format strings.
For example:

```text
Background_AM ""
Background_PM ""
FontDay "Courier10 BT" "Bold Italic" 20.1 150 50 60 0 "Off"
FontMonth "Courier10 BT" "Bold Italic" 10.1 150 50 60 0 "Off"
FontYear "Courier10 BT" "Bold Italic" 10.1 150 50 60 0 "Off"
PositionDay 14 10
PositionMonth 10 35
PositionYear 30 35
DayVisible "On"
MonthVisible "On"
YearVisible "On"
DayString "%d"
MonthString "%b"
YearString "%y"
TextString "%a %d %b %Y"
```

Format strings use `strftime` syntax, so values such as `%A`, `%B` and `%Y`
are also supported. Right-click either the graphical replicant or the compact
text item and choose **Settings…** to open the built-in editor. Its **Help** tab
explains every setting and provides a legend for the supported `%` values. Save
the file, then restart DateReplicant or re-add its replicants to apply the
changes. `TextString` controls the complete date shown by the compact Deskbar
item; existing settings files are upgraded with the default value automatically.

The same right-click menu includes **About DateReplicant…**, which opens the
project's About window with its version, credits, source link and licence
information.

## Heritage

The bundled default background comes from the OBOS GUI prototype. The original
author credited Sergei Dolgov, Roman Kopko and Ilya Nedoluzhko, and can be
contacted at `_sl@mail.ru`.


