#!/bin/sh
echo 'Building deej (development)...'

# shove git commit, version tag into env
GIT_COMMIT=$(git rev-list -1 --abbrev-commit HEAD)
VERSION_TAG=$(git describe --tags --always)
BUILD_TYPE=release

echo 'Embedding build-time parameters:'
echo "- gitCommit $GIT_COMMIT"
echo "- versionTag $VERSION_TAG"
echo "- buildType $BUILD_TYPE"

# make sure the vendored systray patch (webkit2gtk-4.1) is present before building
if [ -d vendor ]; then
    if ! grep -q "webkit2gtk-4.1" vendor/github.com/getlantern/systray/systray_nonwindows.go 2>/dev/null; then
        echo 'Warning: vendor/systray does not have the webkit2gtk-4.1 patch applied.'
        echo 'Run "go mod vendor && git apply patches/systray-webkit41.patch" first.'
        exit 1
    fi
else
    echo 'Error: vendor/ directory not found. Run "go mod vendor" and apply patches/systray-webkit41.patch first.'
    exit 1
fi

go build -mod=vendor -o deej -ldflags "-X main.gitCommit=$GIT_COMMIT -X main.versionTag=$VERSION_TAG -X main.buildType=$BUILD_TYPE" ./pkg/deej/cmd

if [ $? -eq 0 ]; then
    echo 'Done.'
else
    echo 'Error: "go build" exited with a non-zero code. Are you running this script from the root deej directory?'
    exit 1
fi
