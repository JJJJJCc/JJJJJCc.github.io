var app = new Vue({
  el: "#app",
  data: {
    audio: "",
    imgLoaded: false,
    currentlyPlaying: false,
    currentlyStopped: false,
    currentTime: 0,
    checkingCurrentPositionInTrack: "",
    trackDuration: 0,
    currentProgressBar: 0,
    isPlaylistActive: false,
    currentSong: 0,
    debug: false,
    musicPlaylist: [
    {
      title: "R&B all night remake",
      artist: "Chen Ji",
      url: "source/music/rballnight.mp3",
      image: "https://source.unsplash.com/crs2vlkSe98/400x400" },

    {
      title: "Go",
      artist: "Chen Ji",
      url: "source/music/Go.mp3",
      image: "https://source.unsplash.com/35bE_njbG9E/400x400" },

    {
      title: "几分之几cover",
      artist: "Chen Ji",
      url: "source/music/youcompleteme.mp3",
      image: "https://source.unsplash.com/Esax9RaEl2I/400x400" },

      {
      title: "圣诞结Piano&Guitar",
      artist: "Chen Ji",
      url: "source/music/lonly.mp3",
      image: "https://source.unsplash.com/Esax9RaEl2I/400x400" },

      {
      title: "night",
      artist: "Chen Ji",
      url: "source/music/night.mp3",
      image: "https://source.unsplash.com/Esax9RaEl2I/400x400" },

    {
      title: "Secret",
      artist: "Chen Ji",
      url: "source/music/secret.mp3",
      image: "https://source.unsplash.com/j0g8taxHZa0/400x400" }],


    audioFile: "" },

  mounted: function () {
    this.changeSong();
    this.audio.loop = false;
  },
  filters: {
    fancyTimeFormat: function (s) {
      return (s - (s %= 60)) / 60 + (9 < s ? ":" : ":0") + s;
    } },

  methods: {
    togglePlaylist: function () {
      this.isPlaylistActive = !this.isPlaylistActive;
    },
    nextSong: function () {
      if (this.currentSong < this.musicPlaylist.length - 1)
      this.changeSong(this.currentSong + 1);
    },
    prevSong: function () {
      if (this.currentSong > 0) this.changeSong(this.currentSong - 1);
    },
    changeSong: function (index) {
      var wasPlaying = this.currentlyPlaying;
      this.imageLoaded = false;
      if (index !== undefined) {
        this.stopAudio();
        this.currentSong = index;
      }
      this.audioFile = this.musicPlaylist[this.currentSong].url;
      this.audio = new Audio(this.audioFile);
      var localThis = this;
      this.audio.addEventListener("loadedmetadata", function () {
        localThis.trackDuration = Math.round(this.duration);
      });
      this.audio.addEventListener("ended", this.handleEnded);
      if (wasPlaying) {
        this.playAudio();
      }
    },
    isCurrentSong: function (index) {
      if (this.currentSong == index) {
        return true;
      }
      return false;
    },
    getCurrentSong: function (currentSong) {
      return this.musicPlaylist[currentSong].url;
    },
    playAudio: function () {
      if (
      this.currentlyStopped == true &&
      this.currentSong + 1 == this.musicPlaylist.length)
      {
        this.currentSong = 0;
        this.changeSong();
      }
      if (!this.currentlyPlaying) {
        this.getCurrentTimeEverySecond(true);
        this.currentlyPlaying = true;
        this.audio.play();
      } else {
        this.stopAudio();
      }
      this.currentlyStopped = false;
    },
    stopAudio: function () {
      this.audio.pause();
      this.currentlyPlaying = false;
      this.pausedMusic();
    },
    handleEnded: function () {
      if (this.currentSong + 1 == this.musicPlaylist.length) {
        this.stopAudio();
        this.currentlyPlaying = false;
        this.currentlyStopped = true;
      } else {
        this.currentlyPlaying = false;
        this.currentSong++;
        this.changeSong();
        this.playAudio();
      }
    },
    onImageLoaded: function () {
      this.imgLoaded = true;
    },
    getCurrentTimeEverySecond: function (startStop) {
      var localThis = this;
      this.checkingCurrentPositionInTrack = setTimeout(
      function () {
        localThis.currentTime = localThis.audio.currentTime;
        localThis.currentProgressBar =
        localThis.audio.currentTime / localThis.trackDuration * 100;
        localThis.getCurrentTimeEverySecond(true);
      }.bind(this),
      1000);

    },
    pausedMusic: function () {
      clearTimeout(this.checkingCurrentPositionInTrack);
    },
    toggleDebug: function () {
      this.debug = !this.debug;
      document.body.classList.toggle('debug');
    } },

  watch: {
    currentTime: function () {
      this.currentTime = Math.round(this.currentTime);
    } },

  beforeDestroy: function () {
    this.audio.removeEventListener("ended", this.handleEnded);
    this.audio.removeEventListener("loadedmetadata", this.handleEnded);

    clearTimeout(this.checkingCurrentPositionInTrack);
  } });