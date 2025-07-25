#pragma once

#include "itrackeditinteraction.h"
#include "trackedit/trackediterrors.h"
#include "playback/iplayback.h"
#include "playback/iplayer.h"
#include "global/types/secs.h"
#include "modularity/ioc.h"
#include "record/irecordcontroller.h"
#include "au3wrap/internal/progressdialog.h"

namespace au::trackedit {
class TrackeditInteraction : public ITrackeditInteraction, public muse::Injectable
{
    muse::Inject<au::record::IRecordController> recordController;
    muse::Inject<au::playback::IPlayback> playback;

public:
    TrackeditInteraction(std::unique_ptr<ITrackeditInteraction> interaction);

private:
    muse::secs_t clipStartTime(const trackedit::ClipKey& clipKey) const override;
    muse::secs_t clipEndTime(const trackedit::ClipKey& clipKey) const override;
    bool changeClipStartTime(const trackedit::ClipKey& clipKey, secs_t newStartTime, bool completed) override;
    muse::async::Channel<trackedit::ClipKey, secs_t /*newStartTime*/, bool /*completed*/> clipStartTimeChanged() const override;
    bool trimTracksData(const std::vector<trackedit::TrackId>& tracksIds, secs_t begin, secs_t end) override;
    bool silenceTracksData(const std::vector<trackedit::TrackId>& tracksIds, secs_t begin, secs_t end) override;
    bool changeTrackTitle(const trackedit::TrackId trackId, const muse::String& title) override;
    bool changeClipTitle(const trackedit::ClipKey& clipKey, const muse::String& newTitle) override;
    bool changeClipPitch(const ClipKey& clipKey, int pitch) override;
    bool resetClipPitch(const ClipKey& clipKey) override;
    bool changeClipSpeed(const ClipKey& clipKey, double speed) override;
    bool resetClipSpeed(const ClipKey& clipKey) override;
    bool changeClipColor(const ClipKey& clipKey, const std::string& color) override;
    bool changeTracksColor(const TrackIdList& tracksIds, const std::string& color) override;
    bool changeClipOptimizeForVoice(const ClipKey& clipKey, bool optimize) override;
    bool renderClipPitchAndSpeed(const ClipKey& clipKey) override;
    void clearClipboard() override;
    muse::Ret pasteFromClipboard(secs_t begin, bool moveClips, bool moveAllTracks) override;
    bool cutClipIntoClipboard(const ClipKey& clipKey) override;
    bool cutClipDataIntoClipboard(const TrackIdList& tracksIds, secs_t begin, secs_t end, bool moveClips) override;
    bool copyClipIntoClipboard(const trackedit::ClipKey& clipKey) override;
    bool copyNonContinuousTrackDataIntoClipboard(const TrackId trackId, const ClipKeyList& clipKeys, secs_t offset) override;
    bool copyContinuousTrackDataIntoClipboard(const TrackId trackId, secs_t begin, secs_t end) override;
    bool removeClip(const trackedit::ClipKey& clipKey) override;
    bool removeClips(const ClipKeyList& clipKeyList, bool moveClips) override;
    bool removeTracksData(const TrackIdList& tracksIds, secs_t begin, secs_t end, bool moveClips) override;
    bool moveClips(secs_t timePositionOffset, int trackPositionOffset, bool completed, bool&) override;
    bool splitTracksAt(const TrackIdList& tracksIds, std::vector<secs_t> pivots) override;
    bool splitClipsAtSilences(const ClipKeyList& clipKeyList) override;
    bool splitRangeSelectionAtSilences(const TrackIdList& tracksIds, secs_t begin, secs_t end) override;
    bool splitRangeSelectionIntoNewTracks(const TrackIdList& tracksIds, secs_t begin, secs_t end) override;
    bool splitClipsIntoNewTracks(const ClipKeyList& clipKeyList) override;
    bool mergeSelectedOnTracks(const TrackIdList& tracksIds, secs_t begin, secs_t end) override;
    bool duplicateSelectedOnTracks(const TrackIdList& tracksIds, secs_t begin, secs_t end) override;
    bool duplicateClip(const ClipKey& clipKey) override;
    bool duplicateClips(const ClipKeyList& clipKeyList) override;
    bool clipSplitCut(const ClipKey& clipKey) override;
    bool clipSplitDelete(const ClipKey& clipKey) override;
    bool splitCutSelectedOnTracks(const TrackIdList tracksIds, secs_t begin, secs_t end) override;
    bool splitDeleteSelectedOnTracks(const TrackIdList tracksIds, secs_t begin, secs_t end) override;
    bool trimClipLeft(const ClipKey& clipKey, secs_t deltaSec, secs_t minClipDuration, bool completed, UndoPushType type) override;
    bool trimClipRight(const ClipKey& clipKey, secs_t deltaSec, secs_t minClipDuration, bool completed, UndoPushType type) override;
    bool stretchClipLeft(const ClipKey& clipKey, secs_t deltaSec, secs_t minClipDuration, bool completed, UndoPushType type) override;
    bool stretchClipRight(const ClipKey& clipKey, secs_t deltaSec, secs_t minClipDuration, bool completed, UndoPushType type) override;
    muse::secs_t clipDuration(const trackedit::ClipKey& clipKey) const override;
    std::optional<secs_t> getLeftmostClipStartTime(const ClipKeyList& clipKeys) const override;
    bool newMonoTrack() override;
    bool newStereoTrack() override;
    bool newLabelTrack() override;
    bool deleteTracks(const TrackIdList& trackIds) override;
    bool duplicateTracks(const TrackIdList& trackIds) override;
    void moveTracks(const TrackIdList& trackIds, const TrackMoveDirection direction) override;
    void moveTracksTo(const TrackIdList& trackIds, int to) override;

    bool undo() override;
    bool canUndo() override;
    bool redo() override;
    bool canRedo() override;
    bool undoRedoToIndex(size_t index) override;

    bool insertSilence(const TrackIdList& trackIds, secs_t begin, secs_t end, secs_t duration) override;

    bool toggleStretchToMatchProjectTempo(const ClipKey& clipKey) override;

    int64_t clipGroupId(const trackedit::ClipKey& clipKey) const override;
    void setClipGroupId(const trackedit::ClipKey& clipKey, int64_t id) override;
    void groupClips(const trackedit::ClipKeyList& clipKeyList) override;
    void ungroupClips(const trackedit::ClipKeyList& clipKeyList) override;
    ClipKeyList clipsInGroup(int64_t id) const override;

    bool changeTracksFormat(const TrackIdList& tracksIds, trackedit::TrackFormat format) override;
    bool changeTracksRate(const TrackIdList& tracksIds, int rate) override;

    bool swapStereoChannels(const TrackIdList& tracksIds) override;
    bool splitStereoTracksToLRMono(const TrackIdList& tracksIds) override;
    bool splitStereoTracksToCenterMono(const TrackIdList& tracksIds) override;

    muse::ProgressPtr progress() const override;

private:
    template<typename Func, typename ... Args>
    muse::Ret withPlaybackStop(Func method, Args&&... args)
    {
        if (recordController()->isRecording()) {
            return make_ret(trackedit::Err::DisallowedDuringRecording);
        }
        playback()->player()->stop();
        return (m_interaction.get()->*method)(std::forward<Args>(args)...);
    }

    template<typename Func, typename ... Args>
    muse::Ret withProgress(Func&& method, Args&&... args) const
    {
        auto progressDialog = std::make_unique<ProgressDialog>();
        progress()->progressChanged().onReceive(progressDialog.get(),
                                                [&](int64_t current, int64_t total, const std::string&) {
            const auto result = progressDialog->Poll(current, total);
            if (result == ProgressResult::Cancelled) {
                progress()->cancel();
            }
        });

        return method(std::forward<Args>(args)...);
    }

    const std::unique_ptr<ITrackeditInteraction> m_interaction;
};
} // namespace au::trackedit
