#pragma once
/*
 *      Copyright (C) 2012-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "windows/GUIMediaWindow.h"
#include "utils/Observer.h"

#define CONTROL_BTNVIEWASICONS            2
#define CONTROL_BTNSORTBY                 3
#define CONTROL_BTNSORTASC                4
#define CONTROL_BTNGROUPITEMS             5
#define CONTROL_BTNSHOWHIDDEN             6
#define CONTROL_BTNSHOWDELETED            7
#define CONTROL_BTNHIDEDISABLEDTIMERS     8
#define CONTROL_BTNCHANNELGROUPS          28
#define CONTROL_BTNFILTERCHANNELS         31

#define CONTROL_LABEL_HEADER1             29
#define CONTROL_LABEL_HEADER2             30

class CGUIDialogProgressBarHandle;

namespace PVR
{
  enum EPGSelectAction
  {
    EPG_SELECT_ACTION_CONTEXT_MENU   = 0,
    EPG_SELECT_ACTION_SWITCH         = 1,
    EPG_SELECT_ACTION_INFO           = 2,
    EPG_SELECT_ACTION_RECORD         = 3,
    EPG_SELECT_ACTION_PLAY_RECORDING = 4,
  };

  class CPVRChannelGroup;
  typedef std::shared_ptr<CPVRChannelGroup> CPVRChannelGroupPtr;

  class CPVRTimerInfoTag;
  typedef std::shared_ptr<CPVRTimerInfoTag> CPVRTimerInfoTagPtr;

  class CGUIWindowPVRBase : public CGUIMediaWindow, public Observer
  {
  public:
    virtual ~CGUIWindowPVRBase(void);
    virtual void OnInitWindow(void) override;
    virtual void OnDeinitWindow(int nextWindowID) override;
    virtual bool OnMessage(CGUIMessage& message) override;
    virtual bool OnContextButton(const CFileItem &item, CONTEXT_BUTTON button) { return false; };
    virtual bool Update(const std::string &strDirectory, bool updateFilterPath = true) override;
    virtual void UpdateButtons(void) override;
    virtual bool OnAction(const CAction &action) override;
    virtual bool OnBack(int actionID) override;
    virtual bool OpenChannelGroupSelectionDialog(void);
    virtual void Notify(const Observable &obs, const ObservableMessage msg) override;
    virtual void SetInvalid() override;
    virtual bool CanBeActivated() const override;

    static std::string GetSelectedItemPath(bool bRadio);
    static void SetSelectedItemPath(bool bRadio, const std::string &path);

    static bool PlayRecording(CFileItem *item, bool bPlayMinimized = false, bool bCheckResume = true);

  protected:
    CGUIWindowPVRBase(bool bRadio, int id, const std::string &xmlFile);

    virtual std::string GetDirectoryPath(void) = 0;

    virtual void ClearData();

    bool InitChannelGroup(void);
    virtual CPVRChannelGroupPtr GetChannelGroup(void);
    virtual void SetChannelGroup(const CPVRChannelGroupPtr &group);

    virtual bool ActionDeleteChannel(CFileItem *item);
    virtual bool ActionInputChannelNumber(int input);

    virtual void ShowRecordingInfo(CFileItem *item);
    virtual bool UpdateEpgForChannel(CFileItem *item);

    virtual void UpdateSelectedItemPath();

    void RegisterObservers(void);
    void UnregisterObservers(void);

    static CCriticalSection m_selectedItemPathsLock;
    static std::string m_selectedItemPaths[2];

    CCriticalSection m_critSection;
    bool m_bRadio;

  private:
    /*!
     * @brief Show or update the progress dialog.
     * @param strText The current status.
     * @param iProgress The current progress in %.
     */
    void ShowProgressDialog(const std::string &strText, int iProgress);

    /*!
     * @brief Hide the progress dialog if it's visible.
     */
    void HideProgressDialog(void);

    CPVRChannelGroupPtr m_channelGroup;
    XbmcThreads::EndTime m_refreshTimeout;
    CGUIDialogProgressBarHandle *m_progressHandle; /*!< progress dialog that is displayed while the pvr manager is loading */
  };
}
