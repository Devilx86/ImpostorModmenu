package com.devilx86.modmenu;

import android.app.Service;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.os.IBinder;
import android.text.Html;
import android.text.TextUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.List;

public class MenuService extends Service {
    private final String TAG = "IMPOSTORMENU";
    private final String MENU_TITLE = "Impostor Modmenu v1.3";
    private final String MENU_AUTHOR = "Devilx86";
    private final String CONTRIBUTORS = "RevAsnake, Rprop, Cydia Substrate";
    private final String MENU_FOOTER = "<marquee><p style=\"font-size:30\">"
                                        + "<br>"
                                        + "Modded by" + "<p style=\"color:red;\">" + MENU_AUTHOR + "</p> | "
                                        + "<p style=\"color:red;\">" + "https://github.com/Devilx86</p> | "
                                        + "Thanks to " + CONTRIBUTORS + " |"
                                        + "</p></marquee>";


    public native String[] getPlayerNames();

    public native void updateMods(int feature, int value);

    public native void sendLobbyMessage(String message);

    private void addSwitch(String text, Switch.OnCheckedChangeListener on) {
        Switch switchBtn = new Switch(getBaseContext());
        switchBtn.setTextSize(18.0f);
        switchBtn.setBackgroundColor(Color.parseColor("#171E24"));
        switchBtn.setTextColor(Color.parseColor("#DEEDF6"));
        switchBtn.setPadding(10, 20, 0, 20);
        switchBtn.setSwitchPadding(10);
        switchBtn.setText(text);
        switchBtn.setOnCheckedChangeListener(on);
        mMenuBody.addView(switchBtn);
    }

    private void addItem(String text, View.OnClickListener on) {
        TextView label = new TextView(getBaseContext());
        label.setTextColor(Color.WHITE);
        label.setTextSize(18.0f);
        label.setBackgroundColor(Color.parseColor("#171E24"));
        label.setPadding(10, 20, 0, 20);
        label.setText(text);
        label.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch(event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        v.setBackgroundColor(Color.GRAY);
                        break;
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                        v.setBackgroundColor(Color.parseColor("#171E24"));
                        break;
                }
                return false;
            }
        });
        label.setOnClickListener(on);
        label.setClickable(true);

        mMenuBody.addView(label);
    }

    private void addSubtitle(String text) {
        TextView label = new TextView(getBaseContext());
        label.setTextColor(Color.WHITE);
        label.setTextSize(18.0f);
        label.setBackgroundColor(Color.BLACK);
        label.setPadding(10, 20, 0, 20);
        label.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
        label.setPadding(10, 5, 0, 10);
        label.setText(text);

        mMenuBody.addView(label);
    }

    private void addSeekBar(final String text, final int min, final int max, final int arg1) {
        final TextView label = new TextView(getBaseContext());
        label.setTextColor(Color.WHITE);
        label.setTextSize(18.0f);
        label.setBackgroundColor(Color.parseColor("#171E24"));
        label.setPadding(10, 20, 0, 20);
        label.setText(text + ": ");
        mMenuBody.addView(label);

        SeekBar seekBar = new SeekBar(getBaseContext());
        seekBar.setProgress(min);
        seekBar.setMax(max);
        seekBar.setBackgroundColor(Color.parseColor("#171E24"));
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int value, boolean b) {
                if(value < min) {
                    value = min;
                    seekBar.setProgress(value);
                }
                updateMods(arg1, value);
                label.setText(text + ": " + value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        mMenuBody.addView(seekBar);
    }

    private void createMenu() {
        Log.d(TAG, "Creating Menu header");

        // Menu Header
        TextView heading = new TextView(getBaseContext());
        heading.setText(MENU_TITLE);
        heading.setTextColor(Color.RED);
        heading.setBackgroundColor(Color.BLACK);
        heading.setTypeface(Typeface.DEFAULT_BOLD);
        heading.setGravity(17);
        heading.setTextSize(18);
        heading.setPadding(10, 10, 10, 10);
        mExpanded.addView(heading);

        Log.d(TAG, "Creating Menu Body");
        createMenuBody();

        ScrollView scroll = new ScrollView(getBaseContext());
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(mExpanded.getLayoutParams());
        params.weight = 1.0f;
        scroll.setLayoutParams(params);
        scroll.addView(mMenuBody);
        mExpanded.addView(scroll);

        Log.d(TAG, "Creating Menu Footer");
        // Menu Footer
        TextView footer = new TextView(getBaseContext());
        footer.setEllipsize(TextUtils.TruncateAt.MARQUEE);
        footer.setSingleLine(true);
        footer.setSelected(true);
        footer.setText(Html.fromHtml(MENU_FOOTER));
        footer.setTextColor(Color.GRAY);
        footer.setBackgroundColor(Color.BLACK);
        footer.setTypeface(Typeface.DEFAULT_BOLD);
        footer.setGravity(17);
        footer.setTextSize(12);
        footer.setPadding(10, 10, 10, 10);
        mExpanded.addView(footer);
    }

    private void createMenuBody() {
        addSubtitle("Self");
        addSeekBar("Скорость игрока", 0, 100, 46);
        addItem("Максимальное освещение", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(47, 2000);
            }
        });
        addSwitch("Фэйк импостор", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(1, isChecked ? 1 : 0);
            }
        });

        addSwitch("Нет перезарядки убийств", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(2, isChecked ? 1 : 0);
            }
        });
        addSwitch("Показать предателей", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(3, isChecked ? 1 : 0);
            }
        });
        addSwitch("No Emergency Cooldown", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(4, isChecked ? 1 : 0);
            }
        });
        addSwitch("Wall Hack", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(5, isChecked ? 1 : 0);
            }
        });

        addSwitch("Смена цветов", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(6, isChecked ? 1 : 0);
            }
        });

        addSubtitle("Player Options");
        addItem("Teleport to Player", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);

                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(7, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });




        addItem("Прикрепиться к игроку", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.getMenu().add("Detach");
                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(10, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });





        addItem("Fake MedScan", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.getMenu().add("[Stop Animation]");

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            final int index = list.indexOf(menuItem.getTitle());

                            updateMods(9, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });


        addItem("Kick Player", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(13, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });

        addSwitch("Скип голосования", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(14, isChecked ? 1 : 0);
            }
        });

        addItem("Cast Vote", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                // -1 since it does not exist in the list so we can skip instead of player[i]
                menu.getMenu().add("Skip Vote");

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(15, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });

        addItem("репорт умершего", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            final int index = list.indexOf(menuItem.getTitle());

                            updateMods(16, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });


        addItem("Revive Player [Host]", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            final int index = list.indexOf(menuItem.getTitle());

                            updateMods(17, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });



        addItem("Завершить задания", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            final int index = list.indexOf(menuItem.getTitle());

                            updateMods(20, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });



        addSubtitle("Ship");

        addSwitch("Lock All Doors", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(22, isChecked ? 1 : 0);
            }
        });

        addItem("Sabotage Loop", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList("Reactor", "Lights", "Oxygen", "Communications", "Seismic");

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.getMenu().add("Turn Off loop");
                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                        updateMods(23, list.indexOf(menuItem.getTitle()));
                        return false;
                    }
                });
                menu.show();
            }
        });

        addItem("Sabotage all", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(24, 1);
            }
        });

        addItem("Repair (o2, com, rea)", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(25, 1);
            }
        });

        addItem("Play Shield Animation", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(26, 1);
            }
        });

        addItem("Play Weapons Animation", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(26, 6);
            }
        });

        addItem("Play Trash Animation", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(26, 10);
            }
        });

        addSubtitle("Lobby");
        addSwitch("Force Impostor [Host]", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(28, isChecked ? 1 : 0);
            }
        });

        addSeekBar("Impostor Count [Host]", 0, 3, 27);
        addItem("Select Impostor 1 [Host]", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(29, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });


        addItem("Select Impostor 2 [Host]", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(30, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });


        addItem("Select Impostor 3 [Host]", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(31, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });

        addItem("Attach Lobby Plus", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.getMenu().add("Detach");
                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(32, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });

        addItem("Attach Lobby Behind", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.getMenu().add("Detach");
                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(33, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });

        // Quicker way than all to player
        addItem("Teleport All to me", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(34, 1);
            }
        });

        addItem("Teleport All to player", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PopupMenu menu = new PopupMenu(getBaseContext(), v);
                final List<String> list = Arrays.asList(getPlayerNames());

                for(int i = 0; i < list.size(); i++)
                    menu.getMenu().add(list.get(i));

                menu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                       if(!menuItem.getTitle().equals("-")) {
                            updateMods(35, list.indexOf(menuItem.getTitle()));
                        }
                        return false;
                    }
                });
                menu.show();
            }
        });

        addSwitch("Freeze All Players", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(36, isChecked ? 1 : 0);
            }
        });

        addItem("Blame Murder Crew", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(37, 1);
            }
        });

        addItem("Me Murder Crew", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(38, 1);
            }
        });

        addItem("Murder Impostors", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateMods(39, 1);
            }
        });

        addSwitch("Freeze last murderer", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(40, isChecked ? 1 : 0);
            }
        });

        addSwitch("Teleport all to Murderer", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(41, isChecked ? 1 : 0);
            }
        });

        addSwitch("Random Skin Changer", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(42, isChecked ? 1 : 0);
            }
        });

        addSwitch("Random Pet Changer", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(43, isChecked ? 1 : 0);
            }
        });

        addSwitch("Random Hat Changer", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(44, isChecked ? 1 : 0);
            }
        });

        addSwitch("Random Colour Changer [Host]", new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                updateMods(45, isChecked ? 1 : 0);
            }
        });

        addItem("Advertise Menu", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendLobbyMessage(MENU_TITLE + " By " + MENU_AUTHOR);
            }
        });


        addItem("Hide/Unhide icon", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isMenuIconVisible = !isMenuIconVisible;
                if(!isMenuIconVisible)
                    mMenuHeadImageView.setAlpha(0.8f);
                else
                    mMenuHeadImageView.setAlpha(0.0f);
            }
        });

        addItem("Exit", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopSelf();
            }
        });
    }

    // Reference: https://www.androidhive.info/2016/11/android-floating-widget-like-facebook-chat-head/
    private WindowManager mWindowManager;
    private FrameLayout rootFrame;
    private ImageView mMenuHeadImageView;
    private LinearLayout mRootContainer;
    private RelativeLayout mCollapsed;
    private LinearLayout mExpanded;
    private LinearLayout mMenuBody;
    private boolean isMenuIconVisible = true;

    private void initFloatingView() {
        AssetManager assetManager = getBaseContext().getAssets();

        rootFrame = new FrameLayout(getBaseContext());
        mRootContainer = new LinearLayout(getBaseContext());
        mCollapsed = new RelativeLayout(getBaseContext());
        mExpanded = new LinearLayout(getBaseContext());
        mMenuBody = new LinearLayout(getBaseContext());

        /*
            When -1 or -2 is applied, the view fills the screen
            just as it would for match_parent or fill_parent.

            When any number -3 or lower is applied, the behavior is the same
            as wrap_content.
         */
        rootFrame.setLayoutParams(new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
        mRootContainer.setLayoutParams(new RelativeLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
        mRootContainer.setOrientation(LinearLayout.HORIZONTAL);
        mCollapsed.setLayoutParams(new RelativeLayout.LayoutParams(-2, -2));

        mMenuHeadImageView = new ImageView(getBaseContext());
        mMenuHeadImageView.setLayoutParams(new LinearLayout.LayoutParams(150, 150));

        try {
            InputStream istr = assetManager.open("icon.png");
            mMenuHeadImageView.setImageDrawable(Drawable.createFromStream(istr, null));
            mMenuHeadImageView.setAlpha(0.8f);
            istr.close();

            mCollapsed.addView(mMenuHeadImageView);

            mExpanded.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT));
            mExpanded.setOrientation(LinearLayout.VERTICAL);
            mExpanded.setPadding(10, 0, 10, 0);
            mExpanded.setBackgroundColor(Color.BLACK);
            mExpanded.setAlpha(0.7f);

            mMenuBody.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.MATCH_PARENT));
            mMenuBody.setOrientation(LinearLayout.VERTICAL);
            mMenuBody.setBackgroundColor(Color.BLACK);
            //mMenuBody.setBackgroundColor(Color.parseColor("#171E24"));

            //children of layout2 LinearLayout

            createMenu();

            // set visibility
            mMenuHeadImageView.setVisibility(View.VISIBLE);
            mCollapsed.setVisibility(View.VISIBLE);
            mExpanded.setVisibility(View.GONE);


            /*
            ScrollView scroll = new ScrollView(getBaseContext());
            scroll.setLayoutParams(new LinearLayout.LayoutParams(mExpanded.getLayoutParams()));
            scroll.addView(mExpanded);
            */

            // add views
            /*
            TextView lobbyInfo = new TextView(getBaseContext());
            lobbyInfo.setTextColor(Color.BLACK);
            lobbyInfo.setBackgroundColor(Color.WHITE);
            lobbyInfo.setText("Lobby Info\nPlayer 1\nPlayer 2");
            lobbyInfo.setGravity(Gravity.RIGHT);
            lobbyInfo.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
            lobbyInfo.setClickable(false);

            mCollapsed.addView(lobbyInfo);
            */
            mRootContainer.addView(mCollapsed);
            mRootContainer.addView(mExpanded);
            rootFrame.addView(mRootContainer);

            final WindowManager.LayoutParams params;
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                params = new WindowManager.LayoutParams(
                        WindowManager.LayoutParams.WRAP_CONTENT,
                        WindowManager.LayoutParams.WRAP_CONTENT,
                        WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                        PixelFormat.TRANSLUCENT);
            } else {
                //Add the view to the window.
                params = new WindowManager.LayoutParams(
                        WindowManager.LayoutParams.WRAP_CONTENT,
                        WindowManager.LayoutParams.WRAP_CONTENT,
                        WindowManager.LayoutParams.TYPE_PHONE,
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                        PixelFormat.TRANSLUCENT);
            }
            //Specify the chat head position
            //Initially view will be added to top-left corner
            params.gravity = Gravity.TOP | Gravity.LEFT;
            params.x = 0;
            params.y = 300;

            //Add the view to the window
            mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
            mWindowManager.addView(rootFrame, params);

            mMenuHeadImageView.setOnTouchListener(new View.OnTouchListener() {
                private int initialX;
                private int initialY;
                private float initialTouchX;
                private float initialTouchY;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {
                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            initialX = params.x;
                            initialY = params.y;
                            initialTouchX = motionEvent.getRawX();
                            initialTouchY = motionEvent.getRawY();
                            return true;
                        case MotionEvent.ACTION_UP:
                            int Xdiff = (int) (motionEvent.getRawX() - initialTouchX);
                            int Ydiff = (int) (motionEvent.getRawY() - initialTouchY);

                            if (Xdiff < 10 && Ydiff < 10) {
                                if(mExpanded.getVisibility() == View.GONE) {
                                    Log.d("ACTION_UP", "Expanding View...");
                                    mCollapsed.setVisibility(View.VISIBLE);
                                    mExpanded.setVisibility(View.VISIBLE);

                                } else {
                                    Log.d("ACTION_UP", "Collapsing View...");
                                    mExpanded.setVisibility(View.GONE);
                                }
                            }
                            return true;
                        case MotionEvent.ACTION_MOVE:
                            params.x = initialX + (int) (motionEvent.getRawX() - initialTouchX);
                            params.y = initialY + (int) (motionEvent.getRawY() - initialTouchY);
                            mWindowManager.updateViewLayout(rootFrame, params);
                            return true;
                    }
                    return false;
                }
            });
        } catch (IOException e){
            Toast.makeText(getBaseContext(), "Error: Could not load image from assets folder ", Toast.LENGTH_LONG).show();
            Toast.makeText(getBaseContext(), e.toString(), Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return Service.START_NOT_STICKY;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        System.loadLibrary("devilhook");
        initFloatingView();
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        super.onTaskRemoved(rootIntent);
        stopSelf();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mMenuHeadImageView != null)
            mWindowManager.removeView(rootFrame);
        stopSelf();
    }


}