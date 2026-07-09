#!/usr/bin/env python3
"""Recover a GraphBoard component's IDispatch surface from the MSFT type library
embedded in its DLL.

Each component DLL carries a compiled type library (magic ``MSFT``) as a
resource. Its name table (segment-directory slot 9) is a flat list of every
type, interface, method, property and parameter name, each tagged with an
``hreftype`` that groups members under the interface they belong to. That is
exactly the incoming script->component method vocabulary the runtime needs (the
methods a page script calls: ``Sprite_Holder.MoveTo``, ``ChangePhase``, ...),
alongside the base component interface and the outgoing event interface.

This parser reads the name table directly — no COM/oleaut32 calls — so it works
cross-platform on the raw DLL bytes.

Usage:
    python extract_typelib.py <component.dll> [component.dll ...]
    python extract_typelib.py --dir "C:\\Program Files (x86)\\Common Files\\GraphBoard 1.00"
"""
import glob
import os
import struct
import sys


def extract_names(data):
    """Return the ordered [(name, hreftype)] list from a DLL's MSFT name table,
    or None if the DLL has no type library."""
    msft = data.find(b"MSFT")
    if msft < 0:
        return None
    # 15-entry segment directory begins at header+0x54; each entry is
    # (offset, length, res01, res02) DWORDs, offsets relative to the MSFT start.
    seg_base = msft + 0x54
    seg = [struct.unpack_from("<iiii", data, seg_base + i * 16) for i in range(15)]
    name_off, name_len = seg[9][0], seg[9][1]
    if name_off < 0 or name_len <= 0:
        return None

    start = msft + name_off
    end = start + name_len
    p = start
    names = []
    while p < end - 12:
        hreftype = struct.unpack_from("<i", data, p)[0]
        nl = data[p + 8]  # low byte of the namelen/flags dword
        raw = data[p + 12:p + 12 + nl]
        if not (1 <= nl <= 48) or not all(0x20 <= c < 0x7F for c in raw):
            break
        names.append((raw.decode("latin1"), hreftype))
        p += 12 + nl
        if p % 4:  # entries are padded to a 4-byte boundary (with 0x57 bytes)
            p += 4 - (p % 4)
    return names


def _is_type_name(name):
    """A type/interface/struct declaration rather than a member."""
    return (
        (name[0] == "I" and name[1:2].isupper())          # IObject, IEvent, ...
        or (name[0] == "C" and name[1:2].isupper())        # CSpriteHolder1
        or name.startswith("tag")                          # tagRECT
        or name.endswith("Lib")                            # SPRITEHOLDERLib
    )


def group_by_interface(names):
    """Group members by their hreftype value (the type-info they are declared
    in). hreftype -1 marks a parameter/field of the preceding member and is
    dropped. Each group is labelled with the type/interface name that carries
    the same hreftype; the remaining same-href names are its members, in table
    order. Names are interleaved in the table, so grouping is by value, not
    adjacency."""
    order = []            # first-seen order of href values
    members = {}          # href -> [member names]
    label = {}            # href -> interface/type label
    for name, href in names:
        if href == -1:
            continue
        if href not in members:
            members[href] = []
            order.append(href)
        if _is_type_name(name) and href not in label:
            label[href] = name
        else:
            members[href].append(name)
    return [(label.get(h, f"type@{h}"), h, members[h]) for h in order]


def report(path):
    data = open(path, "rb").read()
    names = extract_names(data)
    dll = os.path.basename(path)
    print(f"\n{'=' * 62}\n{dll}")
    if names is None:
        print("  (no MSFT type library)")
        return
    for label, href, members in group_by_interface(names):
        if not members:
            continue
        print(f"  {label}  [hreftype {href}]")
        for m in members:
            print(f"      {m}")


def main():
    args = sys.argv[1:]
    paths = []
    if args and args[0] == "--dir":
        paths = sorted(glob.glob(os.path.join(args[1], "*.dll")))
    else:
        paths = args
    if not paths:
        print(__doc__)
        sys.exit(2)
    for p in paths:
        report(p)


if __name__ == "__main__":
    main()
