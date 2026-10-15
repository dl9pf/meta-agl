#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# aglsetup.sh-muscle-memory wrapper around `kas-container shell`, for local exploration only.
#
#   scripts/aglsetup.sh -m qemux86-64 agl-demo agl-devel
#
# Lives in meta-agl (not AGL-ci) - this is the split-repo successor to the monorepo's
# aglsetup.sh. meta-agl/kas/feature/*.yml (core features) colon-join directly, no cross-repo
# mechanism needed. A feature from meta-agl-demo/meta-agl-devel needs that sibling repo
# checked out alongside meta-agl - if it isn't, kas fetches it itself (same mechanism kas
# already uses for bitbake/meta-flutter/etc - no custom clone-detection logic here, just telling
# kas where to look). Proven live this session: both the pre-cloned-sibling and the
# fetch-it-yourself cases produce identical, successful builds.
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
cd "$REPO_ROOT"
# kas resolves `repos: meta-agl: path: meta-agl` as $KAS_WORK_DIR/meta-agl - so KAS_WORK_DIR
# must be meta-agl's *parent* directory (where it sits as a named sibling of meta-agl-demo/
# meta-agl-devel), never meta-agl's own root, or that path doubles up and resolves nowhere.
# Every kasfile path handed to kas-container is therefore prefixed with "$SELF/" below.
WORKSPACE_ROOT="$(dirname "$REPO_ROOT")"
SELF="$(basename "$REPO_ROOT")"

# Which repo each feature lives in, post-split. Update this table when a feature moves or a
# new one is added - this is the one place that needs to know the whole layout.
declare -A FEATURE_REPO=(
  [agl-app-framework]=meta-agl
  [agl-drm-lease]=meta-agl
  [agl-flutter]=meta-agl
  [agl-ic]=meta-agl
  [agl-kuksa-val]=meta-agl
  [agl-pipewire]=meta-agl
  [agl-selinux]=meta-agl
  [agl-ic-slint]=meta-agl
  [agl-demo]=meta-agl-demo
  [agl-kvm]=meta-agl-demo
  [agl-ic-container]=meta-agl-demo
  [agl-uhmi-demo]=meta-agl-demo
  [agl-devel]=meta-agl-devel
  [agl-package-management]=meta-agl-devel
  [agl-uhmi]=meta-agl-devel
  [agl-xen]=meta-agl-devel
)

# h3ulcb/m3ulcb proprietary R-Car gfx/multimedia package copy (classic aglsetup.sh's
# 50_setup.sh equivalent - imperative, no kas mechanism for this). h3ulcb/h3ulcb-kf/m3ulcb/
# m3ulcb-kf only, not the -nogfx variants.
run_machine_setup_hooks() {
  local machine="$1"
  case "$machine" in
    h3ulcb|h3ulcb-kf|m3ulcb|m3ulcb-kf)
      local hook="meta-agl-bsp/meta-rcar-gen3/scripts/setup_mm_packages.sh"
      if [ -f "$hook" ]; then
        echo "$(basename "$0"): running proprietary R-Car package hook for $machine"
        (
          export METADIR="$PWD"
          # shellcheck source=/dev/null
          source "$hook"
          copy_mm_packages
        ) || echo "$(basename "$0"): WARNING: proprietary package setup failed/incomplete" \
                  "for $machine - see docs/setup.md for what's required on your workstation" >&2
      else
        echo "$(basename "$0"): WARNING: $hook not found, skipping proprietary package setup for $machine" >&2
      fi
      ;;
  esac
}

# kas-container's own arg parser expects `--runtime-args VALUE` as two separate words - see
# ci/scripts/_kas_runtime_args.sh in AGL-ci for the fuller version this is trimmed from.
kas_runtime_args() {
  printf -- '--runtime-args --security-opt --runtime-args label=disable '
  if [ -n "${AGL_SSTATE_DIR:-}" ]; then
    mkdir -p "${AGL_SSTATE_DIR}"
    printf -- '--runtime-args -v --runtime-args %s:%s --runtime-args -e --runtime-args AGL_SSTATE_DIR=%s ' \
      "${AGL_SSTATE_DIR}" "${AGL_SSTATE_DIR}" "${AGL_SSTATE_DIR}"
  fi
  if [ -n "${AGL_DL_DIR:-}" ]; then
    mkdir -p "${AGL_DL_DIR}"
    printf -- '--runtime-args -v --runtime-args %s:%s --runtime-args -e --runtime-args AGL_DL_DIR=%s ' \
      "${AGL_DL_DIR}" "${AGL_DL_DIR}" "${AGL_DL_DIR}"
  fi
}

usage() {
  cat <<EOF
Usage: $(basename "$0") [-m|--machine MACHINE] [-b|--builddir DIR] [feature [feature ...]]

Drops you into an interactive kas-container shell for MACHINE plus the given features. Any
feature from meta-agl-demo/meta-agl-devel auto-fetches that sibling repo if it isn't already
checked out alongside meta-agl. Run bitbake yourself once inside, e.g. \`bitbake agl-image-minimal\`.

  -m, --machine MACHINE   default: qemux86-64
  -b, --builddir DIR      use DIR as the kas build directory instead of the shared build/
  -h, --help              this message

Available machines:
$(for f in kas/machine/*.yml; do b="$(basename "$f" .yml)"; printf '  %s\n' "$b"; done)

Available features:
$(for f in "${!FEATURE_REPO[@]}"; do printf '  %s\n' "$f"; done | sort)
EOF
}

MACHINE="qemux86-64"
BUILDDIR=""
FEATURES=()
while [ $# -gt 0 ]; do
  case "$1" in
    -m|--machine) MACHINE="$2"; shift 2 ;;
    -b|--builddir) BUILDDIR="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    --) shift; FEATURES+=("$@"); break ;;
    -*) echo "$(basename "$0"): unknown option: $1" >&2; usage >&2; exit 2 ;;
    *) FEATURES+=("$1"); shift ;;
  esac
done

if [ ! -f "kas/machine/$MACHINE.yml" ]; then
  echo "$(basename "$0"): no kas/machine/$MACHINE.yml - see --help for the list" >&2
  exit 2
fi

# Classify requested features: core ones colon-join directly (same repo), demo/devel ones need
# the sibling repo declared + included via kas's own cross-repo include mechanism.
CORE_FEATURES=()
declare -A NEEDED_SIBLINGS=()
for f in "${FEATURES[@]}"; do
  [ -z "$f" ] && continue
  repo="${FEATURE_REPO[$f]:-}"
  if [ -z "$repo" ]; then
    echo "$(basename "$0"): unknown feature \"$f\" - see --help for the list" >&2
    exit 2
  elif [ "$repo" = "meta-agl" ]; then
    CORE_FEATURES+=("$f")
  else
    NEEDED_SIBLINGS["$repo"]+="$f "
  fi
done

run_machine_setup_hooks "$MACHINE"

# Local write-path (relative to cwd=$REPO_ROOT); KASFILES below references it with the
# "$SELF/" prefix instead, since kas resolves every colon-joined path against $KAS_WORK_DIR
# (=$WORKSPACE_ROOT, meta-agl's parent), not against this script's own cwd.
ENTRY_FILE="kas/.aglsetup-entry.yml"
rm -f "$ENTRY_FILE"

KASFILES="$SELF/kas/base.yml:$SELF/kas/machine/$MACHINE.yml"
for f in "${CORE_FEATURES[@]}"; do
  KASFILES="$KASFILES:$SELF/kas/feature/$f.yml"
done

# Generated per-run, untracked (see .gitignore) - not committed, not hand-maintained. Declares
# each needed sibling repo (path:-only if already cloned alongside meta-agl, url:+path if not -
# kas fetches it itself, same as any other dependency) and includes the requested feature(s)
# from it via kas's documented {repo:, file:} cross-repo include form.
if [ "${#NEEDED_SIBLINGS[@]}" -gt 0 ]; then
  {
    echo "header:"
    echo "  version: 22"
    echo "  includes:"
    for repo in "${!NEEDED_SIBLINGS[@]}"; do
      for f in ${NEEDED_SIBLINGS[$repo]}; do
        echo "    - repo: $repo"
        echo "      file: kas/feature/$f.yml"
      done
    done
    echo "repos:"
    for repo in "${!NEEDED_SIBLINGS[@]}"; do
      echo "  $repo:"
      if [ -d "../$repo" ]; then
        echo "    path: $repo"
      else
        echo "    url: https://github.com/automotive-grade-linux/$repo"
        echo "    branch: master"
        echo "    path: $repo"
      fi
      # bare declaration defaults to treating the repo root itself as a layer, which is wrong
      # for meta-agl-devel (its subdirs are the real layers) - harmless no-op for meta-agl-demo
      # (overridden again by whatever the included feature's own repos: block adds).
      echo "    layers: {}"
    done
  } > "$ENTRY_FILE"
  KASFILES="$KASFILES:$SELF/$ENTRY_FILE"
fi

if [ -n "${AGL_FLOATING:-}" ]; then
  KASFILES="$KASFILES:$SELF/kas/floating.yml"
else
  KASFILES="$KASFILES:$SELF/kas/pins.yml"
fi

if [ -n "$BUILDDIR" ]; then
  export KAS_BUILD_DIR="$BUILDDIR"
fi

echo "aglsetup.sh: kas shell $KASFILES"
echo "aglsetup.sh: common targets once inside: agl-image-boot, agl-image-minimal," \
     "agl-image-weston, agl-image-compositor"
# kas-container itself checks each colon-joined path's existence relative to the invoking
# shell's cwd (before the container even starts) - cwd must match KAS_WORK_DIR for the
# "$SELF/..."-prefixed paths above to resolve. Everything earlier (hooks, sibling checks,
# writing $ENTRY_FILE) intentionally ran from $REPO_ROOT instead - cd here, last, right before
# handing off to kas-container.
cd "$WORKSPACE_ROOT"
# shellcheck disable=SC2046
KAS_TARGET="${KAS_TARGET:-agl-image-minimal}" KAS_WORK_DIR="$WORKSPACE_ROOT" \
  exec kas-container $(kas_runtime_args) shell "$KASFILES"
