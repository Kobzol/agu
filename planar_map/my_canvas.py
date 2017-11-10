package planarmap;

import java.awt.Color;
import static java.lang.System.out;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import javax.swing.BorderFactory;
import javax.swing.JLabel;

public class MyCanvas extends JLabel implements MouseListener {

  private PlanMap planMap;
  private Edge nearestEdge = null;
  private boolean firstVertex = true;
  
  boolean firstSplitVertex = false;
  boolean secondSplitVertex = false;

  public MyCanvas() {
    planMap = new PlanMap();
    this.addMouseListener(this);
    this.setBorder(BorderFactory.createLoweredBevelBorder());
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);

    Graphics2D g2 = (Graphics2D) g;
    planMap.draw(g2);

    if (nearestEdge != null) {
      g2.setColor(Color.RED);
      nearestEdge.draw(g2);
      g2.setColor(Color.BLACK);
    }
    
    if (planMap.eCount > 3) {
      g2.setColor(Color.YELLOW);
      planMap.firstF.draw(g2);
      g2.setColor(Color.BLACK);
    }
    
    g2.dispose();
  }

  public void mouseClicked(MouseEvent e) {
    Coord coord = new Coord(e.getX(), e.getY());

    // split edge
    if (e.getButton() == MouseEvent.BUTTON1) {
      if (firstVertex) {
        planMap.firstV.coord.x = e.getX();
        planMap.firstV.coord.y = e.getY();
        firstVertex = !firstVertex;
      }
      if (nearestEdge == null) {
        planMap.splitEdge(planMap.lastE, coord);
      } else {
        planMap.splitEdge(nearestEdge, coord);
      }
    }
    
    //select edge to split
    if (e.getButton() == MouseEvent.BUTTON3) {
      nearestEdge = planMap.nearestEdge(coord);
      out.println(nearestEdge);
    }

    // printing info about planar map
    if (e.getButton() == MouseEvent.BUTTON2) {
      Face f = planMap.splitFace(planMap.firstF, planMap.firstE, planMap.lastE);
      out.println(planMap);
      out.println(f);
    }
    this.repaint();
  }
